#include "SortApplication.h"
#include "qpushbutton.h"
#include "qmessagebox.h"

SortApplication::SortApplication(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->resize(350, 350);
    QPushButton* button = new QPushButton(this);
    button->setGeometry(50, 50, 200, 200);
    button->setText("专用按钮");
    connect(button, &QPushButton::clicked, this, [=]() { QMessageBox::information(this, "test", "测试用例"); });
}

SortApplication::~SortApplication()
{}

