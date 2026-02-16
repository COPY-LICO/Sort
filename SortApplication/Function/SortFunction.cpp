#include "SortFunction.h"

SortFunction::SortFunction(QWidget* parent = nullptr) : QObject(parent)
{
	//获取操作管理对象
	this->manager = ManagerMent::GetInstance();
}


//根据ManagerMent的信息来决定采用的分类方法
bool SortFunction::SureSortOperator()
{
	InfoGroup* infoType = manager->GetOperatorType();
	
	if (infoType->chooseForm == ChooseForm::Sort) //采用分类
	{
		if (infoType->sortType == SortType::byTimePoints) // 根据时间分类
		{
			//获取信息 - 调用时间分类函数
			this->SortFileByTimePoint();
		}
		else if (infoType->sortType == SortType::byFileTypes) // 根据文件类型分类
		{
			//获取信息 - 调用类型分类函数
			this->SortFileByFileType();
		}
		else if (infoType->sortType == SortType::byFileSize) // 根据文件大小分类
		{
			//获取信息 - 调用大小分类函数
			this->SortFileByFileSize();
		}
		else if (infoType->sortType == SortType::bySameFileName) // 根据文件名分类
		{
			//获取信息 - 调用文件名分类函数
			this->SortFileByFileName();
		}
	}
	else if (infoType->chooseForm == ChooseForm::Rename) //采用重命名
	{
		//未开发
		return false;
	}

	return false;
}

//时间区间分类
bool SortFunction::SortFileByTimePoint()
{
	return false;
}

//文件类型分类
bool SortFunction::SortFileByFileType()
{
	return false;
}

//文件大小分类
bool SortFunction::SortFileByFileSize()
{
	return false;
}

//文件名相同筛选分类
bool SortFunction::SortFileByFileName()
{
	return false;
}