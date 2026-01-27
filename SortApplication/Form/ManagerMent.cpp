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

	//operatorForm输入错误
	return false;
}

InfoGroup* ManagerMent::GetOperatorType()
{
	//返回操作类型指针
	return &(this->infoGroup);
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

// 按文件名删除文件
bool ManagerMent::DeleteFileByName(const QString& fileName)
{
	qDebug() << "ManagerMent:";
	this->PrintAllFilesInfo();
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

//以下为调试代码
void ManagerMent::PrintAllFilesInfo()
{
	//打印所有文件数据
	vector<Files>::iterator fileIt = this->GetLastFilesPathGroup();

	for (int i = this->GetNowFilesNum(); i > 0; i--)
	{
		qDebug() << (*fileIt).fileName << "   " << (*fileIt).suffix << "   " << (*fileIt).modifyTime << "    " << (*fileIt).size << "   " << (*fileIt).filePath << "\n";
		if (fileIt != this->_fileGroup.begin())
		{
			fileIt--;
		}
	}
}

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

