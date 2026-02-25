#include "ManagerMent.h"
#include "qfileinfo.h"
#include "qdebug.h"
#include "qdatetime.h"

using std::vector;

ManagerMent::ManagerMent(QObject* parent) : QObject(parent)
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

//url为文件路径
bool ManagerMent::SaveFiles(QString url)
{
	//获取存入文件信息
	QFileInfo info(url);
	if (info.exists())
	{
		Files tempfiles;
		//填入文件大小
		tempfiles.size = int(info.size());
		//填入文件路径
		tempfiles.filePath = url;
		//填入文件名称
		tempfiles.prefix = info.baseName();
		//填入文件后缀
		tempfiles.suffix = info.suffix();
		//填入文件名
		tempfiles.fileName = info.fileName();
		//填入文件修改日期
		QDateTime tempTime = info.lastModified();
		tempfiles.modifyTime = tempTime.toString("yyyy-MM-dd hh:mm:ss");
		//存入vector
		_fileGroup.push_back(tempfiles);

		return true;
	}
	else
	{
		return false;
	}

	return false;
}

vector<Files>::iterator ManagerMent::GetLastFilesPathGroup()
{
	//判断是否有未处理的路径
	if (!this->_fileGroup.empty())
	{
		return --this->_fileGroup.end(); //--回到最后一个元素
	}
	else
	{
		//已经到底了，指向空
		return this->_fileGroup.end();
	}

	return this->_fileGroup.end();
}

vector<QString>& ManagerMent::GetBackSuffix()
{
	//返回后缀库
	return this->_backSuffix;
}

bool ManagerMent::DeleteLastFiles()
{
	//删除最后一个文件的所有信息
	if (!this->_fileGroup.empty())
	{
		//存在文件，删除
		this->_fileGroup.pop_back();
		return true;
	}
	else
	{
		//不存在文件，跳过
		return false;
	}

	return false;
}

int ManagerMent::GetNowFilesNum()
{
	return _fileGroup.size();
}

//operatorForm为选择的模式，operatorType为该模式下选择的方案。若不传入参数则执行为初始化，返回值false
bool ManagerMent::SaveOperatorType(int operatorForm, int operatorType)
{

	if (operatorForm == ChooseForm::Sort)
	{
		if (operatorType < 0 || operatorType > 3)
		{
			//operatorType输入错误
			return false;
		}

		//选择分类操作
		this->infoGroup.chooseForm = operatorForm;
		this->infoGroup.sortType = operatorType;
		this->infoGroup.renameType = chooseNull;//置空
		return true;
	}
	else if (operatorForm == ChooseForm::Rename)
	{
		if (operatorType < 0 || operatorType > 2)
		{
			//operatorType输入错误
			return false;
		}

		//选择重命名操作
		this->infoGroup.chooseForm = operatorForm;
		this->infoGroup.renameType = operatorType;
		this->infoGroup.sortType = chooseNull;//置空
		return true;
	}
	else if (operatorForm == chooseNull && operatorType == chooseNull)
	{
		//初始化 返回false
		this->infoGroup.chooseForm = chooseNull;
		this->infoGroup.renameType = chooseNull;
		this->infoGroup.sortType = chooseNull;
		return false;
	}

	//operatorForm输入错误
	return false;
}

InfoGroup* ManagerMent::GetOperatorType()
{
	//返回操作类型指针
	return &(this->infoGroup);
}

DetailInfo* ManagerMent::GetOperatorContent()
{
	//返回操作内容指针
	return &(this->detailGroup);
}

vector<QString> ManagerMent::TheBackSuffix()
{
	//储存已有的后缀名
	return 
	{
		".txt", ".doc", ".docx", ".xls", ".xlsx",
		".ppt", ".pptx", ".pdf", ".jpg", ".png",
		".gif", ".mp3", ".mp4", ".avi", ".zip",
		".cpp", ".h", ".py", "md",
		".log", ".xml", ".json"
	};
}

//判断文件是否重复
bool ManagerMent::IsFileExistByPath(const QString& filePath)
{
	if (_fileGroup.empty())
		return false;

	for (auto it = _fileGroup.begin(); it != _fileGroup.end(); ++it)
	{
		if (it->filePath == filePath)
		{
			return true;
		}
	}
	return false;
}

// 按文件名删除文件
bool ManagerMent::DeleteFileByName(const QString& fileName)
{
	//qDebug() << "ManagerMent:";
	//this->PrintAllFilesInfo();
	for (auto it = _fileGroup.begin(); it != _fileGroup.end(); ++it)
	{
		qDebug() << it->fileName << ":" << fileName << "\n";
		if (it-> fileName == fileName)
		{
			_fileGroup.erase(it);
			return true;
		}
	}
	return false;
}

//清空所有文件
void ManagerMent::ClearAllFiles()
{
	_fileGroup.clear();
}

//存入分类操作类型文本框的内容 - 默认全部为空 - 在点击开始按钮发送分类命令前调用
//不输入任何数据表明初始化后端数据
//返回false表明初始化，返回true表明赋值成功
bool ManagerMent::SaveOperatorContent(bool byYear, bool byYear_Month, int largeFile, int smallFile, QString sortName, std::vector<QString> tempType, QString renameContent)
{
	if (byYear != false || byYear_Month != false || largeFile > 0 || smallFile > 0 || !sortName.isEmpty() == true || !tempType.empty() == true || !renameContent.isEmpty() == true)
	{
		this->detailGroup.byYear = byYear;
		this->detailGroup.byYear_Month = byYear_Month;
		this->detailGroup.largeFile = largeFile;
		this->detailGroup.smallFile = smallFile;
		this->detailGroup.typeGroup = tempType;
		this->detailGroup.sortName = sortName;
		this->detailGroup.renameContent = renameContent;
		return true; // 赋值成功
	}
	else
	{
		this->detailGroup.byYear = byYear;
		this->detailGroup.byYear_Month = byYear_Month;
		this->detailGroup.largeFile = largeFile;
		this->detailGroup.smallFile = smallFile;
		this->detailGroup.typeGroup = tempType;
		this->detailGroup.sortName = sortName;
		this->detailGroup.renameContent = renameContent;
		return false; // 初始化成功
	}
	return false;
}

//存入历史操作记录
bool ManagerMent::SaveRecordFiles(QString oldFileName, QString newFileName, QString oldFilePath, QString newFilePath)
{
	RecordFiles tempRecord;
	tempRecord.oldFileName = oldFileName;
	tempRecord.newFileName = newFileName;
	tempRecord.oldFileName = oldFilePath;
	tempRecord.newFilePath = newFilePath;

	//存入数据 
	//传入数据不完整，返回false
	if (oldFileName == "" || newFileName == "" || oldFilePath == "" || newFilePath == "")
	{
		return false;
	}
	this->_recordFileGroup.push_back(tempRecord);
	return true;
}

//返回存入历史操作记录的指向最后一个元素的迭代器
std::vector<RecordFiles>::iterator ManagerMent::GetRecordFilesGroup()
{
	if (!this->_recordFileGroup.empty())
	{
		//指向最后一个元素
		return --this->_recordFileGroup.end();
	}
	else
	{
		//指向空
		return this->_recordFileGroup.end();
	}

	return this->_recordFileGroup.end();;
}

void ManagerMent::ClearAllRecordFiles()
{
	this->_recordFileGroup.clear();
}

//以下为调试代码

//打印所有文件数据
void ManagerMent::PrintAllFilesInfo()
{

	vector<Files>::iterator fileIt = this->GetLastFilesPathGroup();

	for (int i = this->GetNowFilesNum(); i > 0; i--)
	{
		qDebug() << (*fileIt).fileName << "   " << (*fileIt).suffix << "   " << (*fileIt).modifyTime << "    " << (*fileIt).size << "   " << (*fileIt).filePath;
		if (fileIt != this->_fileGroup.begin())
		{
			fileIt--;
		}
	}
}

//手动存入文件数据
void ManagerMent::SaveFilesForTest(QString name, QString suffix, QString time, QString path, int size)
{
	Files tempFile;
	tempFile.fileName = name;
	tempFile.suffix = suffix;
	tempFile.filePath = path;
	tempFile.modifyTime = time;
	tempFile.size = size;
	this->_fileGroup.push_back(tempFile);
}

//打印目前的操作内容
void ManagerMent::PrintAllOperation()
{
	qDebug() << "选择的操作类型：" << this->infoGroup.chooseForm;
	qDebug() << "采用的分类模式：" << this->infoGroup.sortType;
	qDebug() << "采用的重命名模式：" << this->infoGroup.renameType;
	qDebug() << "分类操作细节" << '\n' << "启用时间间隔状态：" << "按年份：" << this->detailGroup.byYear << "     按月份" << this->detailGroup.byYear_Month;
	qDebug() << "已读取的文件类型:";
	for (const QString& temp : this->detailGroup.typeGroup)
		qDebug() << temp << "   ";

	qDebug() << "划分大小参数：" << "large：" << this->detailGroup.largeFile << "    " << "small: " << this->detailGroup.smallFile;
	qDebug() << "已经读取的文件名：" << this->detailGroup.sortName;
	
	// qDebug() << "重命名操作细节" << '\n' << 
}

//打印所有存入的历史文件记录
void ManagerMent::PrintAllRecordFilesInfo()
{
	vector<RecordFiles>::iterator fileIt = this->GetRecordFilesGroup();

	for (int i = this->GetNowFilesNum(); i > 0; i--)
	{
		qDebug() << "oldFileName: " << (*fileIt).oldFileName << "   newFileName: " << (*fileIt).newFileName;
		qDebug() << "oldFilePath: " << (*fileIt).oldFilePath << "   newFilePath: " << (*fileIt).newFilePath;
		if (fileIt != this->_recordFileGroup.begin())
		{
			fileIt--;
		}
	}
}
