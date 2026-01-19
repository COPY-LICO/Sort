#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SortApplication.h"
#include <QFileDialog>

class SortApplication : public QMainWindow
{
    Q_OBJECT

public:
    SortApplication(QWidget* parent = nullptr);
    
    //事件过滤器处理点击悬停事件
    bool eventFilter(QObject* watched, QEvent* event) override;
    
    
    ~SortApplication();


private slots:
    //打开文件夹
    void OpenFileDialog();

private:
    Ui::SortApplicationClass ui;
};