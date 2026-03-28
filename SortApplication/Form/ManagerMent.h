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
	//临时文件导入长期存储
	bool ImportRecordToAllGroup();



	//存入文件地址
	bool SaveFiles(QString);
	//存入当前操作类型 - 无参数传入为初始化操作类型
	bool SaveOperatorType(int = chooseNull, int = chooseNull);
	//存入当前操作内容 - 无参数传入为初始化操作内容
	bool SaveOperatorContent(bool = false, bool = false, int = chooseNull, int = chooseNull, std::vector<QString> = {}, QString = "");
	//保存历史文件记录
	bool SaveRecordFiles(QString oldFileName, QString newFileName, QString oldFilePath, QString newFilePath);
	//保存分类文件夹的记录路径
	bool SaveMovePath(QString);
	//导出长期存储中的指定记录
	std::vector<IntegratedContent> OutPortRecordToAllGroup(int index);



	//获取文件数组中最后存入文件的迭代器
	std::vector<Files>::iterator GetLastFilesPathGroup();
	//返回后缀库引用
	std::vector<QString>& GetBackSuffix();
	//获取历史文件记录 - 迭代器 - 指向最后一个文件
	std::vector<RecordFiles>::iterator GetRecordFilesGroup();
	//获取当前操作类型
	InfoGroup* GetOperatorType();
	//获取当前操作内容细节
	DetailInfo* GetOperatorContent();
	//获取分类文件夹的记录路径
	QString GetMovePath();
	//读入最新的历史记录进入临时记录的信息
	bool GetLastRecordToTempRecord();



	//返回当前文件数目
	int GetNowFilesNum();
	//获取当前历史操作数目
	int GetNowRecordNum();
	//返回历史记录中的文件数量
	int GetRecordFilesNum();
	//返回长期存储中的记录数量
	int GetAllRecordFilesNum();



	//按文件名删除文件
	bool DeleteFileByName(const QString& fileName);
	//删除最后一个文件所有信息
	bool DeleteLastFiles();
	//清空最后历史记录文件
	void ClearAllRecordFiles();
	//清空所有文件
	void ClearAllFiles();
	//移除长期存储中的指定记录
	bool DeleteRecordToAllGroup(int index);




	//安全函数 - 判断历史记录文件是否为空
	bool IsRecordFilesEmpty();
	//安全函数 - 判断文件迭代器是否达到头部顶点
	bool IsFileItPosFilesTop(std::vector<Files>::iterator it);
	//安全函数 - 判断历史文件迭代器是否达到头部顶点
	bool IsRecordFileItPosFilesTop(std::vector<RecordFiles>::iterator it);
	//安全函数 - 判断文件是否重复
	bool IsFileExistByPath(const QString& filePath);



	//调试代码 - 打印所有存入文件的信息
	void PrintAllFilesInfo();
	//调式代码 - 手动输入文件数据
	void SaveFilesForTest(QString,QString,QString,QString,int);
	//调式代码 - 打印识别的操作内容
	void PrintAllOperation();
	//调试代码 - 打印所有存入的历史文件记录
	void PrintAllRecordFilesInfo();
	//调试代码 - 导出临时操作所有记录
	std::vector<IntegratedContent> ReturnOperationAllFileRecord(bool);


	//索引自减
	void IndexDecrement();
	//索引自增
	void IndexIncrement();
	//获得当前索引
	int GetIndex();

	//撤回成功值初始化
	void MakeWithdrawSuccessFalse();
	//获取撤回成功值
	bool GetWithdrawSuccess();
	//修改撤回成功值
	void ModifyWithdrawSuccess(bool);


//信号函数
signals:
	//开始分类信号
	void StartOperator();
signals:
	//撤回分类信号
	void StartWithDrawOperator();
signals:
	//执行完毕信号
	void EndOperator();

public slots:
	//调用 - 完成执行系列操作
	void LastOperator();

private:

	//私有化构造函数防止类外实例化
	ManagerMent(QObject* parent = nullptr);
	//后缀名填充函数
	static std::vector<QString> TheBackSuffix();
	//导出临时操作所有记录
	std::vector<IntegratedContent> ReturnOperationAllFileRecord();
	//文件储存器
	std::vector<Files> _fileGroup;
	//后缀库
	std::vector<QString> _backSuffix;
	//历史文件临时储存
	std::vector<RecordFiles> _recordFileGroup;
	//历史文件储存
	std::vector<std::vector<IntegratedContent>> _recordFileAllGroup;
	//操作内容
	InfoGroup infoGroup; // 操作类型
	DetailInfo detailGroup; // 操作细节
	//分类路径
	QString movePath;
	//索引
	int _index = 0;
	//撤回成功值
	bool _withdrawSuccess = false;
};

