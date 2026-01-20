#include "SortApplication.h"
#include "ManagerMent.h"
#include <QTimer>

SortApplication::SortApplication(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


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
    auto it = ManagerMent::GetInstance();


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
    QString folderPath = QFileDialog::getOpenFileName(
        this,
        "choose files",
        QDir::homePath(),
        "All Files(*.*)"
      
    );
    if (!folderPath.isEmpty())
    {
        qDebug() << "选中的文件夹路径：" << folderPath;
        // 这里可以添加后续逻辑，比如显示路径到界面、读取文件夹内容等
    }

}



SortApplication::~SortApplication()
{
}
