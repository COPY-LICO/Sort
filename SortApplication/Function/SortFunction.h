#pragma once
#include <qwidget.h>
#include "ManagerMent.h"


class SortFunction : public QObject
{
	Q_OBJECT
public:
	//构造函数
	SortFunction(QWidget* parent);
	//分类函数
	//时间点分类函数
	bool SortFileByTimePoint();
	//文件类型分类函数
	bool SortFileByFileType();
	//文件大小分类函数
	bool SortFileByFileSize();
	//文件名筛选分类函数
	bool SortFileByFileName();

public slots:
	//分类槽函数 - 监听ManagerMent - StartOperator()信号
	//判断使用的分类方法
	bool SureSortOperator();

private:
	ManagerMent* manager;
};

