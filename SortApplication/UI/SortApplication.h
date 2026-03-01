#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SortApplication.h"
#include <QFileDialog>
#include <QMessageBox>

class SortApplication : public QMainWindow
{
    Q_OBJECT

public:
    SortApplication(QWidget* parent = nullptr);
    
    //重写事件过滤器
    bool eventFilter(QObject* watched, QEvent* event) override;
    
    //添加单个文件到列表
    void AddFiletoItem(const QString &filePath);

    //文件类型分类面板
    void InitFileTypePanel();

    //新增历史记录
    void AddHistoryItem(const QString& operName, const QString& operMode, const QString& operContent, const QString& operTime, int fileCount);

    //操作完成清除文件储存及显示
    void ClearItem();
    
    ~SortApplication();


private slots:
    //打开文件
    void OpenFileDialog();
    //右键触发菜单
    void OnCustomContextMenuRequested(const QPoint& pos);
    // 右键删除item
    void OnDeleteItemByRightClick();
    //右键清空所有item
    void OnClearItemByRightClick();
    //radioButton控制checkBox可选
    void OnFileTypeCheckBoxToggled(bool checked);
    //开始执行操作
    void OnStartButtonClicked();
    //选择储存文件的文件夹
    void OnSelectFolderClicked();
    //执行撤回操作
    void OnWithdrawClicked();

private:
    Ui::SortApplicationClass ui;
    // 临时存储右键点击的item（用于删除）
    QListWidgetItem* _rightClickedItem = nullptr;
    //存储所有滚动区域的CheckBox的指针便于批量初始化状态
    QList<QCheckBox*>   _typeCheckBoxList;
};