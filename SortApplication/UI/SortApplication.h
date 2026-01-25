#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SortApplication.h"
#include <QFileDialog>

class SortApplication : public QMainWindow
{
    Q_OBJECT

public:
    SortApplication(QWidget* parent = nullptr);
    
    //重写事件过滤器
    bool eventFilter(QObject* watched, QEvent* event) override;
    
    //添加单个文件到列表
    void AddFiletoItem(const QString &filePath);
    
    ~SortApplication();


private slots:
    //打开文件
    void OpenFileDialog();

private:
    Ui::SortApplicationClass ui;
};