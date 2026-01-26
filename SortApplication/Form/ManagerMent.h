#pragma once
#include <string>
#include <qstring.h>
#include <qobject.h>
#include <vector>
#include "Files.h"
#include "InfoGroup.h"
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
	bool SaveFiles(QString);
	//获取文件数组中最后存入文件的迭代器
	std::vector<Files>::iterator GetLastFilesPathGroup();
	//返回当前文件数目
	int GetNowFilesNum();
	//删除最后一个文件所有信息
	bool DeleteLastFiles();
	//返回后缀库引用
	std::vector<QString>& GetBackSuffix();
	//存入当前操作类型
	bool SaveOperatorType(int,int);
	//获取当前操作类型
	InfoGroup* GetOperatorType();


	//调试代码 - 打印所有存入文件的信息
	void PrintAllFilesInfo();
	//调式代码 - 手动输入文件数据
	void SaveFilesForTest(QString,QString,QString,QString,int);

	// 按文件名删除文件
	bool DeleteFileByName(const QString& fileName);

//信号函数
signals:
	//开始分类信号
	void StartOperator();



private:

	//私有化构造函数防止类外实例化
	ManagerMent(QObject* parent = nullptr);
	//后缀名填充函数
	static std::vector<QString> TheBackSuffix();
	//文件储存器
	std::vector<Files> _fileGroup;
	//后缀库
	std::vector<QString> _backSuffix;
	//操作类型
	InfoGroup infoGroup;

};

