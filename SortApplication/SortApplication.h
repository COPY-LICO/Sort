#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SortApplication.h"

class SortApplication : public QMainWindow
{
    Q_OBJECT

public:
    SortApplication(QWidget *parent = nullptr);
    ~SortApplication();

private:
    Ui::SortApplicationClass ui;
};

