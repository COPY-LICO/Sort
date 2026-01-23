#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SortApplication.h"
#include <QFileDialog>

class SortApplication : public QMainWindow
{
    Q_OBJECT

public:
    SortApplication(QWidget* parent = nullptr);
    
    //�¼���������������ͣ�¼�
    bool eventFilter(QObject* watched, QEvent* event) override;
    
    
    ~SortApplication();


private slots:
    //���ļ���
    void OpenFileDialog();

private:
    Ui::SortApplicationClass ui;
};