#include "ManagerMent.h"

ManagerMent::ManagerMent(QObject* parent = nullptr) : QObject(parent), _filePath("")
{
	//私有化构造函数用于处理对象
}

ManagerMent* ManagerMent::GetInstance()
{
	//获取返回的单例对象指针
	static ManagerMent instance;
	return &instance;
}

void ManagerMent::SaveFilesPath(string url)
{
	//存入文件路径
	this->_filePath = url;
}

string ManagerMent::GetFilesPath()
{
	//获取文件路径
	return this->_filePath;
}