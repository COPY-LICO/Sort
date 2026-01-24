#include "SortFunction.h"

SortFunction::SortFunction(QWidget* parent = nullptr) : QObject(parent)
{
	//获取操作管理对象
	this->manager = ManagerMent::GetInstance();
}