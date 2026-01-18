#include "ManagerMent.h"

ManagerMent::ManagerMent(QObject* parent = nullptr) : QObject(parent)
{
	//私有化构造函数用于处理对象
	this->InitalBackSuffix();
}

void ManagerMent::InitalBackSuffix()
{
	//初始化后缀库
	this->_backSuffix = TheBackSuffix();
}

ManagerMent* ManagerMent::GetInstance()
{
	//获取返回的单例对象指针
	static ManagerMent instance;
	return &instance;
}

void ManagerMent::SaveFilesPath(QString url)
{
	//存入文件路径
	this->_fileGroup.push_back(url);
}

QString ManagerMent::GetFilesPath()
{
	//获取文件路径
	QString url;
	//判断是否有未处理的路径
	if (!this->_fileGroup.empty())
	{
		url = this->_fileGroup.back();
		this->_fileGroup.pop_back();
	}
	else
	{
		url = "";
	}
	return url;
}

vector<QString>& ManagerMent::GetBackSuffix()
{
	//返回后缀库
	return this->_backSuffix;
}

vector<QString> ManagerMent::TheBackSuffix()
{
	//储存已有的后缀名
	return 
	{
		".txt",".cpp",".h",".exe",".doc"
	};
}