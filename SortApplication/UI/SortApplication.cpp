#include "SortApplication.h"
#include "ManagerMent.h"
#include <QTimer>
#include <QSize>
#include <QListWidgetItem>
#include <QFileIconProvider>
#include <QMenu> 
#include <QMessageBox>
#include <QScrollArea>
#include <QCheckBox>
#include <QButtonGroup>

SortApplication::SortApplication(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //启动ManagerMent监听
    ManagerMent* manager = ManagerMent::GetInstance();



    // 获取当前窗口标志，移除最大化按钮标志，保留其他按钮
    Qt::WindowFlags flags = this->windowFlags();
    // 移除最大化按钮（Qt::WindowMaximizeButtonHint）
    flags &= ~Qt::WindowMaximizeButtonHint;
    // 保留最小化按钮（Qt::WindowMinimizeButtonHint）和关闭按钮（Qt::WindowCloseButtonHint）
    flags |= Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint;
    // 应用新的窗口标志
    this->setWindowFlags(flags);

    // 刷新窗口（设置标志后需调用show确保生效）
    this->show();


       //文件分类类型面板调用
    InitFileTypePanel();


    /*===================file_Widget===================*/
    
    //实现点击fileSelectorWidget与iconLabel_CloudRecord、textLabel_select1、textLabel_select2任意一个都打开文件夹：
    //实现四者统一，让四个都走同一个过滤器
    ui.fileSelectorWidget->installEventFilter(this);
    ui.iconLabel_CloudRecord->installEventFilter(this);
    ui.textLabel_select1->installEventFilter(this);
    ui.textLabel_select2->installEventFilter(this);

    //改变光标
    ui.fileSelectorWidget->setCursor(Qt::PointingHandCursor);
    ui.iconLabel_CloudRecord->setCursor(Qt::PointingHandCursor);
    ui.textLabel_select1->setCursor(Qt::PointingHandCursor);
    ui.textLabel_select2->setCursor(Qt::PointingHandCursor);
    



    /*===================Func_groupBox===================*/
    
    //分别连接两个QRadioButton和QStackedWidget的两页（lambda）
    connect(ui.sortRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            ui.stackedWidget->setCurrentIndex(0);

            // 1. 清空Rename组所有RadioButton选中状态
            ui.prefix_radioButton->setChecked(false);
            ui.suffix_radioButton->setChecked(false);
            ui.unifyName_radioButton->setChecked(false);

            // 2. 禁用Rename组所有输入框 + 清空内容
            ui.prefix_Input->setEnabled(false);
            ui.suffix_Input->setEnabled(false);
            ui.unifyName_Input->setEnabled(false);
            ui.prefix_Input->clear();
            ui.suffix_Input->clear();
            ui.unifyName_Input->clear();
        }
        });
    connect(ui.renameRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            ui.stackedWidget->setCurrentIndex(1);

            //清空Sort组所有RadioButton选中状态
            ui.size_radioButton->setChecked(false);
            ui.time_radioButton->setChecked(false);
            ui.type_radioButton->setChecked(false);
            ui.name_radioButton->setChecked(false);

            //禁用并清空name输入框
            ui.nameSort_Input->setEnabled(false);
            ui.nameSort_Input->clear();

            //禁用并清空size输入框
            ui.fileSizeSmall_Input->setEnabled(false);
            ui.fileSizeSmall_Input->clear();
            ui.fileSizeLarge_Input->setEnabled(false);
            ui.fileSizeLarge_Input->clear();

            //清空/禁用time分组下的子RadioButton
            ui.byYear_radioButton->setChecked(false);
            ui.byYear_radioButton->setEnabled(false);
            ui.byYear_radioButton->setCheckable(false);
            ui.byMonth_radioButton->setChecked(false);
            ui.byMonth_radioButton->setEnabled(false);
            ui.byMonth_radioButton->setCheckable(false);

            //清空/禁用type分组下的所有复选框
            for (QCheckBox* checkBox : _typeCheckBoxList) {
                checkBox->setChecked(false);
                checkBox->setEnabled(false);
            }
        }
        });

    //设置默认状态
    ui.sortRadioButton->setChecked(true);


    /*===================sort_groupBox===================*/
 
    //设置全部按钮默认状态
    ui.size_radioButton->setChecked(false);
    ui.time_radioButton->setChecked(false);
    ui.type_radioButton->setChecked(false);
    ui.name_radioButton->setChecked(false);

    //连接name_radioButton和name_Input，只有选中前者，后者才可输入：
    // 1. 设置输入框默认状态：单选按钮未选中，输入框禁用
    ui.nameSort_Input->setEnabled(false);

   //选中其他RadioButton时清空nameSort_Input
        auto clearNameInput = [=]() {
        ui.nameSort_Input->clear();
        ui.nameSort_Input->setEnabled(false);
        };


    // 2. 绑定信号槽   
    // name RadioButton选中时启用输入框（内容不清空，仅切换其他时清空）
    connect(ui.name_radioButton, &QRadioButton::toggled, this, [=](bool checked) {
        ui.nameSort_Input->setEnabled(checked);
        qDebug() << ui.byYear_radioButton->isChecked();
        if (!checked) clearNameInput();
    });




    /*===================rename_groupBox===================*/
    //设置全部按钮默认状态
    ui.prefix_radioButton->setChecked(false);
    ui.suffix_radioButton->setChecked(false);
    ui.unifyName_radioButton->setChecked(false);
    
    //连接三个radioButton和三个LineEdit输入框，前者选中后者才可输入：
    // 1. 设置输入框默认状态：单选按钮未选中，输入框禁用
    ui.prefix_Input->setEnabled(false);
    ui.suffix_Input->setEnabled(false);
    ui.unifyName_Input->setEnabled(false);

    // 2. 绑定信号槽   
    connect(ui.prefix_radioButton, &QRadioButton::toggled, ui.prefix_Input, &QLineEdit::setEnabled);
    connect(ui.suffix_radioButton, &QRadioButton::toggled, ui.suffix_Input, &QLineEdit::setEnabled);
    connect(ui.unifyName_radioButton, &QRadioButton::toggled, ui.unifyName_Input, &QLineEdit::setEnabled);

    // 选中prefix时，清空另外两个输入框
    connect(ui.prefix_radioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            ui.suffix_Input->clear();
            ui.unifyName_Input->clear();
            ui.prefix_Input->setEnabled(true);
            ui.suffix_Input->setEnabled(false);
            ui.unifyName_Input->setEnabled(false);
        }
        else if (!ui.suffix_radioButton->isChecked() && !ui.unifyName_radioButton->isChecked()) {
            ui.prefix_Input->clear();
            ui.prefix_Input->setEnabled(false);
        }
        });

    // 选中suffix时，清空另外两个输入框
    connect(ui.suffix_radioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            ui.prefix_Input->clear();
            ui.unifyName_Input->clear();
            ui.suffix_Input->setEnabled(true);
            ui.prefix_Input->setEnabled(false);
            ui.unifyName_Input->setEnabled(false);
        }
        else if (!ui.prefix_radioButton->isChecked() && !ui.unifyName_radioButton->isChecked()) {
            ui.suffix_Input->clear();
            ui.suffix_Input->setEnabled(false);
        }
        });

    // 选中unifyName时，清空另外两个输入框
    connect(ui.unifyName_radioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            ui.prefix_Input->clear();
            ui.suffix_Input->clear();
            ui.unifyName_Input->setEnabled(true);
            ui.prefix_Input->setEnabled(false);
            ui.suffix_Input->setEnabled(false);
        }
        else if (!ui.prefix_radioButton->isChecked() && !ui.suffix_radioButton->isChecked()) {
            ui.unifyName_Input->clear();
            ui.unifyName_Input->setEnabled(false);
        }
        });


    /*===================time_groupBox===================*/
 
    ui.byYear_radioButton->setChecked(false);
    ui.byMonth_radioButton->setChecked(false);

    //绑定信号槽
    connect(ui.time_radioButton, &QRadioButton::toggled, [=](bool checked) {
        ui.byYear_radioButton->setEnabled(ui.time_radioButton->isChecked());
        ui.byYear_radioButton->setCheckable(ui.time_radioButton->isChecked());
        if (!ui.time_radioButton->isChecked()) {
            ui.byYear_radioButton->setChecked(false);
        }
        });

    connect(ui.time_radioButton, &QRadioButton::toggled, [=](bool checked) {
        ui.byMonth_radioButton->setEnabled(ui.time_radioButton->isChecked());
        ui.byMonth_radioButton->setCheckable(ui.time_radioButton->isChecked());
        if (!ui.time_radioButton->isChecked()) {
            ui.byMonth_radioButton->setChecked(false);
        }
        });




    /*===================ListWidget===================*/
    
    //隐藏滚动条
    ui.selectedFiles_listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.historyRecord_listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //添加右键删除选中功能
    //设置菜单右键列表自定义
    ui.selectedFiles_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //绑定右键点击信号到槽函数
    connect(ui.selectedFiles_listWidget, &QListWidget::customContextMenuRequested,
        this, &SortApplication::OnCustomContextMenuRequested);


    /*===================size_groupBox===================*/


    //连接size_radioButton和两个size_Input，只有选中前者，后者才可输入：
    //设置输入框默认状态：单选按钮未选中，输入框禁用
    ui.fileSizeSmall_Input->setEnabled(false);
    ui.fileSizeLarge_Input->setEnabled(false);

    //选中其他RadioButton时清空
    auto clearSmallSizeInput = [=]() {
        ui.fileSizeSmall_Input->clear();
        ui.fileSizeSmall_Input->setEnabled(false);
        };
    auto clearLargeSizeInput = [=]() {
        ui.fileSizeLarge_Input->clear();
        ui.fileSizeLarge_Input->setEnabled(false);
        };

    //绑定信号槽   
    connect(ui.size_radioButton, &QRadioButton::toggled, this, [=](bool checked) {
        ui.fileSizeSmall_Input->setEnabled(checked);
        if (!checked) clearSmallSizeInput();
        });
    connect(ui.size_radioButton, &QRadioButton::toggled, this, [=](bool checked) {
        ui.fileSizeLarge_Input->setEnabled(checked);
        if (!checked) clearLargeSizeInput();
        });



}

//事件过滤器
bool SortApplication::eventFilter(QObject* watched, QEvent* event)
{

    //处理点击事件
    if ((watched == ui.fileSelectorWidget || watched == ui.iconLabel_CloudRecord ||watched == ui.textLabel_select1 || 
        watched == ui.textLabel_select2) && event->type() == QEvent::MouseButtonPress)
    {
        OpenFileDialog();   //调用打开文件夹函数
        return true;    //避免重复处理
    }

    return QMainWindow::eventFilter(watched, event);    //返回父类事件过滤器继续处理其他事件

}

//打开文件夹
void SortApplication::OpenFileDialog()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(
        this,
        "choose files",
        QDir::homePath(),
        "All Files(*.*)"
      
    );

    //如果选中了文件，逐个添加到列表
    if (!filePaths.isEmpty())
    {
        bool is_saved = false;

        for (const QString& url : filePaths)
        {
            AddFiletoItem(url);            
        }
    }
}

void SortApplication::AddFiletoItem(const QString& filePath)
{

    //获取单例对象
    ManagerMent* _manager = ManagerMent::GetInstance();

    //重复存储直接返回
     if (_manager->IsFileExistByPath(filePath))
    {
         return;
    }

    //存储文件
    bool isSaved = _manager->SaveFiles(filePath);

    if (isSaved)
    {
        _manager->PrintAllFilesInfo();
        QFileInfo fileInfo(filePath);


        //创建列表项
        QListWidgetItem* item = new QListWidgetItem(ui.selectedFiles_listWidget);
        item->setSizeHint(QSize(0, 58));    //设置项宽自适应，高45


        //创建自定义Widget来显示内容
        QWidget* fileWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(fileWidget);  //水平排布
        layout->setContentsMargins(8, 8, 8, 8);
        layout->setSpacing(12);
        fileWidget->setStyleSheet("background-color: transparent; border: none;");


        //文件图标
        QLabel* iconLabel = new QLabel();
        QFileIconProvider iconProvider;
        QIcon fileIcon = iconProvider.icon(fileInfo);
        iconLabel->setPixmap(fileIcon.pixmap(32, 32));
        layout->addWidget(iconLabel);
        iconLabel->setStyleSheet("background-color: transparent; border: none;");

        // 文件信息（名称+大小+类型）
        QWidget* infoWidget = new QWidget();
        QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
        infoLayout->setContentsMargins(0, 0, 0, 0);
        infoLayout->setSpacing(2);
        infoWidget->setStyleSheet("background-color: transparent; border: none;");

        // 文件名（过长省略）
        QLabel* nameLabel = new QLabel(fileInfo.fileName());
        nameLabel->setStyleSheet("font-weight: bold; color: #333333;");
        nameLabel->setObjectName("fileNameLabel"); // 关键：唯一标识
        QString fileName = fileInfo.fileName();
        QFontMetrics metrics(nameLabel->font());
        QString elidedName = metrics.elidedText(fileName, Qt::ElideRight, 160); 
        //nameLabel->setText(elidedName);
        infoLayout->addWidget(nameLabel);


        // 文件大小+类型
        double fileSizeKB = static_cast<double>(fileInfo.size()) / 1024.0;

        QString sizeText = QString("%1 KB · %2")  
            .arg(fileSizeKB, 0, 'f', 2)          //大小 
            .arg(fileInfo.suffix().isEmpty() ? "Unknown type" : fileInfo.suffix().toLower());   //类型
        QLabel* sizeTypeLabel = new QLabel(sizeText);
        sizeTypeLabel->setStyleSheet("font-size: 12px; color: #666666; border: none;");
        sizeTypeLabel->setObjectName("fileSizeTypeLabel"); // 区分标识
        infoLayout->addWidget(sizeTypeLabel);

        layout->addWidget(infoWidget);
        layout->addStretch();

        //将自定义Widget设置到列表项
        ui.selectedFiles_listWidget->setItemWidget(item, fileWidget);


        // 核心：仅保留一层淡灰外边框，选中无任何额外框
        ui.selectedFiles_listWidget->setStyleSheet(R"(
    /* 列表容器：无边框、透明背景，仅控制item间距 */
    QListWidget {
        border: none;
        background-color: transparent;
    }

    /* item默认状态 */
    QListWidget::item {
        background-color: #FFFFFF;
        border: 1px solid rgb(0, 170, 255); 
        border-radius: 6px; 

    }

    /* 悬停状态 */
    QListWidget::item:hover {
        background-color: rgb(245,245,245);
    }

    /* 选中状态 */
    QListWidget::item:selected,
    QListWidget::item:selected:focus {
        background-color: rgb(245,245,245); 
        border: 1px solid rgb(0, 170, 255); 
        outline: none; /* 去掉选中虚线框 */

    }
)");
        //为item之间设置间距
        ui.selectedFiles_listWidget->setSpacing(1);
        // 保证item可选中，但无焦点虚线框（平衡交互和样式）
        ui.selectedFiles_listWidget->setFocusPolicy(Qt::NoFocus);

        //更新文件数量
        ui.textLabel_Selected->setText(QString("The File you Selected (%1)").arg(_manager->GetNowFilesNum()));
    }
}

//右键触发菜单
void SortApplication::OnCustomContextMenuRequested(const QPoint& pos)
{
    //右键点击的控件坐标转换为item坐标，找到对应的item
    _rightClickedItem = ui.selectedFiles_listWidget->itemAt(pos);

    //右键点击空白处不响应
    if (!_rightClickedItem)
    {
        return;
    }

    //创建右键菜单，添加删除选项
    QMenu menu(this); 
    QAction* deleteAction = new QAction("Delete", &menu);
    menu.addAction(deleteAction);

    //绑定删除信号到槽函数
    connect(deleteAction, &QAction::triggered, this, &SortApplication::OnDeleteItemByRightClick);

    //添加清空选项
    QAction* clearAction = new QAction("Clear", &menu);
    menu.addAction(clearAction);

    //绑定清空信号到槽函数
    connect(clearAction, &QAction::triggered, this, &SortApplication::OnClearItemByRightClick);


    //在右键位置显示菜单
    menu.exec(ui.selectedFiles_listWidget->mapToGlobal(pos));

}

// 右键删除item
void SortApplication::OnDeleteItemByRightClick()
{
    if (_rightClickedItem == nullptr)
    {
        qDebug() << "There are no file items to be deleted.";
        return;
    }
     
    //获取右键点击对应文件信息
    QWidget* itemWidget = ui.selectedFiles_listWidget->itemWidget(_rightClickedItem);
    QLabel* nameLabel = itemWidget->findChild<QLabel*>("fileNameLabel");
    if (!nameLabel)
    {
        qDebug() << "Unable to obtain the file name.";
        return;
    }

    QString fileName = nameLabel->text();

    //从ManagerMent中删除对应文件
    ManagerMent* _manager = ManagerMent::GetInstance();
    bool deleteSuccess = _manager->DeleteFileByName(fileName);
    qDebug() << deleteSuccess;
    //删除成功
    if (deleteSuccess)
    {
        //释放内存，删除UI中的item
        delete itemWidget;
        delete _rightClickedItem;
        _rightClickedItem = nullptr;    //置空

        //更新文件数量
        ui.textLabel_Selected->setText(QString("The File you Selected (%1)").arg(_manager->GetNowFilesNum()));
    }
    
}

//右键清空所有item
void SortApplication::OnClearItemByRightClick()
{
    if (_rightClickedItem == nullptr)
    {
        qDebug() << "There are no file items to be deleted.";
        return;
    }

    //添加弹窗，防止误操作
    int ret = QMessageBox::question(
        this,
        "Confirm to clear",
        "Are you sure you want to clear it?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );
    if (ret != QMessageBox::Yes)
    {
        return;
    }

    //清空_fileGroup
    ManagerMent* _manager = ManagerMent::GetInstance();
    _manager->ClearAllFiles();

    //清空UI中的所有item
    while (ui.selectedFiles_listWidget->count() > 0)
    {
        QListWidgetItem* item = ui.selectedFiles_listWidget->takeItem(0);
        QWidget* itemWidget = ui.selectedFiles_listWidget->itemWidget(item);
        if (itemWidget)
        {
            delete itemWidget;
        }
        delete item;
    }

    //清空临时变量
    _rightClickedItem = nullptr;

    //更新文件数量
    ui.textLabel_Selected->setText(QString("The File you Selected (%1)").arg(_manager->GetNowFilesNum()));
}

//文件类型分类面板
void SortApplication::InitFileTypePanel()
{
    // 连接信号槽
    connect(ui.type_radioButton, &QRadioButton::toggled, this, &SortApplication::OnFileTypeCheckBoxToggled);


    // 创建滚动区域内容容器并绑定到scrollArea
    QWidget* scrollContent = new QWidget(ui.fileTypes_scrollArea);
    QGridLayout* contentLayout = new QGridLayout(scrollContent);
    contentLayout->setContentsMargins(10, 10, 10, 10);
    contentLayout->setSpacing(12); // 调整间距，避免分散
    contentLayout->setRowStretch(0, 0); // 取消自动拉伸，紧凑排布


    // 设置滚动区域样式
    ui.fileTypes_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏垂直滚动条
    ui.fileTypes_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏水平滚动条
    ui.fileTypes_scrollArea->setWidgetResizable(true);
    ui.fileTypes_scrollArea->setFrameStyle(QFrame::NoFrame);

    // 将内容容器绑定到scrollArea
    ui.fileTypes_scrollArea->setWidget(scrollContent);


    // 批量创建文件类型checkBox
    QStringList fileTypes = {
        ".txt", ".doc", ".docx", ".xls", ".xlsx",
        ".ppt", ".pptx", ".pdf", ".jpg", ".png",
        ".gif", ".mp3", ".mp4", ".avi", ".zip",
        ".cpp", ".h", ".py", "md",
        ".log", ".xml", ".json"
    };

    int row = 0;   // 行索引
    int col = 0;   // 列索引（0和1，每两个换一行）
    const int cols = 2; // 固定2列

    // 循环创建checkbox
    for (const QString& type : fileTypes)
    {
        QCheckBox* checkBox = new QCheckBox(type, scrollContent);

        //调整样式表，确保勾选框显示
        checkBox->setStyleSheet(R"(
            QCheckBox {
                font-size: 13px; 
                color:black;
                margin: 0px 0px; /* 调整内外边距，避免文字覆盖勾选框 */
                spacing: 5px; /* 勾选框和文字的间距 */
            }
            QCheckBox::indicator { /* 显式设置勾选框样式，确保可见 */
                width: 11px;
                height: 11px;
                border: 1px solid black;
                border-radius: 2px;
            }
            QCheckBox::indicator:checked { /* 勾选状态的样式 */

                image: url(:/SortApplication/images/iconfont_CheckBox.png);
            }
          
        )");

        checkBox->setEnabled(false);    // 初始禁用

        // GridLayout添加控件：每两个一行
        contentLayout->addWidget(checkBox, row, col);
        col++;
        if (col >= cols) { // 列数到2，换行
            col = 0;
            row++;
        }

        _typeCheckBoxList.append(checkBox);
    }

    // 最后一行如果只有1个控件，补全行拉伸
    if (col != 0) {
        contentLayout->setColumnStretch(col, 1); // 空列拉伸，保持双列对齐
    }
    contentLayout->setRowStretch(row + 1, 1); // 最后一行下方拉伸，避免整体分散
}

//radioButton控制checkBox可选
void SortApplication::OnFileTypeCheckBoxToggled(bool checked)
{
    for (QCheckBox* checkBox : _typeCheckBoxList)
    {
        checkBox->setEnabled(checked);
        // 未选中时清空残留勾选状态
        if (!checked && checkBox->isChecked())
        {
            checkBox->setChecked(false);
        }
    }
}


SortApplication::~SortApplication()
{
}
