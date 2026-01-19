#pragma once
#include <string>
#include <qstring.h>
#include <qobject.h>
#include <vector>
//这是一个全局管理类，里面所有函数都采用单例形式，可在全局调用负责启动API接口

class ManagerMent : public QObject
{
	Q_OBJECT
public:

	//获取ManagerMent单例
	static ManagerMent* GetInstance();
	//初始化后缀库
	void InitalBackSuffix();
	//存入文件地址
	bool SaveFiles(QString,int);
	//获取地址数组中最后存入文件的迭代器
	std::vector<QString>::iterator GetLastFilesPathGruop();
	//获取大小数组中最后存入文件的迭代器
	std::vector<int>::iterator GetLastFilesSizeGroup();
	//返回当前文件数目
	int GetNowFilesNum();
	//删除最后一个文件所有信息
	void DeleteLastFiles();
	//返回后缀库引用
	std::vector<QString>& GetBackSuffix();



	//后缀名填充函数
	static std::vector<QString> TheBackSuffix();

	//调试代码 - 打印所有存入文件的信息
	void PrintAllFilesInfo();

private:
	//私有化构造函数防止类外实例化
	ManagerMent(QObject* parent);
	//文件地址储存器
	std::vector<QString> _filePathGroup;
	//后缀库
	std::vector<QString> _backSuffix;
	//文件大小储存器
	std::vector<int> _filesSizeGruop;
};

