#pragma once
#include <string>
#include <qstring.h>
#include <qobject.h>
#include <vector>
//这是一个全局管理类，里面所有函数都采用单例形式，可在全局调用负责启动API接口

using std::vector;

class ManagerMent : public QObject
{
	Q_OBJECT
public:

	//获取ManagerMent单例
	static ManagerMent* GetInstance();
	//初始化后缀库
	void InitalBackSuffix();
	//存入文件地址
	void SaveFilesPath(QString);
	//获取文件地址
	QString GetFilesPath();
	//返回后缀库引用
	vector<QString>& GetBackSuffix();

	//后缀名填充函数
	static vector<QString> TheBackSuffix();

private:
	//私有化构造函数防止类外实例化
	ManagerMent(QObject* parent);
	//私有文件数据
	vector<QString> _fileGroup;
	//后缀库
	vector<QString> _backSuffix;
};

