#pragma once
#include <string>
#include <qobject.h>
//这是一个全局管理类，里面所有函数都采用静态形式，可在全局调用负责启动API接口

using std::string;

class ManagerMent : public QObject
{
	Q_OBJECT
public:

	//获取ManagerMent单例
	static ManagerMent* GetInstance();
	//存入文件地址
	void SaveFilesPath(string);
	//获取文件地址
	string GetFilesPath();

private:
	//私有化构造函数防止类外实例化
	ManagerMent(QObject* parent);
	//私有文件数据
	string _filePath;
};

