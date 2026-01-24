#pragma once
#include <qwidget.h>
#include "ManagerMent.h"


class SortFunction : public QObject
{
	Q_OBJECT
public:
	//构造函数
	SortFunction(QWidget* parent);

public slots:
	//分类槽函数 - 监听ManagerMent - StartOperator()信号
	//void SortFiles();

private:
	ManagerMent* manager;
};

