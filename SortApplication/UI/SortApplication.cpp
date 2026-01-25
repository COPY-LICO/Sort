#include "SortApplication.h"
#include "ManagerMent.h"
#include <QTimer>
#include <QSize>
#include <QListWidgetItem>
#include <QFileIconProvider>


SortApplication::SortApplication(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //启动ManagerMent监听
    ManagerMent* manager = ManagerMent::GetInstance();

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
        }
    });
    connect(ui.renameRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            ui.stackedWidget->setCurrentIndex(1);
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

    // 2. 绑定信号槽   
    connect(ui.name_radioButton, &QRadioButton::toggled, ui.nameSort_Input, &QLineEdit::setEnabled);


    //rename_groupBox
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


    /*===================ListWidget===================*/
    
    //隐藏滚动条
    ui.selectedFIles_listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.historyRecord_listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


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

    //存储文件
    bool isSaved = _manager->SaveFiles(filePath);

    if (isSaved)
    {
        QFileInfo fileInfo(filePath);


        //创建列表项
        QListWidgetItem* item = new QListWidgetItem(ui.selectedFIles_listWidget);
        item->setSizeHint(QSize(0, 60));    //设置项宽自适应，高45


        //创建自定义Widget来显示内容
        QWidget* fileWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(fileWidget);  //水平排布
        layout->setContentsMargins(8, 8, 8, 8);
        layout->setSpacing(12);


        //文件图标
        QLabel* iconLabel = new QLabel();
        QFileIconProvider iconProvider;
        QIcon fileIcon = iconProvider.icon(fileInfo);
        iconLabel->setPixmap(fileIcon.pixmap(32, 32));
        layout->addWidget(iconLabel);


        // 文件信息（名称+大小+类型）
        QWidget* infoWidget = new QWidget();
        QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
        infoLayout->setContentsMargins(0, 0, 0, 0);
        infoLayout->setSpacing(2);


        // 文件名（过长省略）
        QLabel* nameLabel = new QLabel(fileInfo.fileName());
        nameLabel->setStyleSheet("font-weight: bold; color: #333333;");
        QString fileName = fileInfo.fileName();
        QFontMetrics metrics(nameLabel->font());
        QString elidedName = metrics.elidedText(fileName, Qt::ElideRight, 160); 
        nameLabel->setText(elidedName);
        infoLayout->addWidget(nameLabel);

        // 文件大小+类型

        double fileSizeKB = static_cast<double>(fileInfo.size()) / 1024.0;

        QString sizeText = QString("%1 KB · %2")  
            .arg(fileSizeKB, 0, 'f', 2)           
            .arg(fileInfo.suffix().isEmpty() ? "Unknown type" : fileInfo.suffix().toLower());

        QLabel* sizeTypeLabel = new QLabel(sizeText);
        sizeTypeLabel->setStyleSheet("font-size: 12px; color: #666666; border: none;");
        infoLayout->addWidget(sizeTypeLabel);

        layout->addWidget(infoWidget);
        layout->addStretch();

        // 5. 将自定义Widget设置到列表项
        ui.selectedFIles_listWidget->setItemWidget(item, fileWidget);


    }
}

SortApplication::~SortApplication()
{
}
