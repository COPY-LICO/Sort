#include "SortApplication.h"
#include "ManagerMent.h"
#include <QTimer>

SortApplication::SortApplication(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


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
    
}

SortApplication::~SortApplication()
{
}
