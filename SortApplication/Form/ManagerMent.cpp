#include "ManagerMent.h"
#include "qfileinfo.h"
#include "qdebug.h"

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
		//存入文件大小
		this->_filesSizeGruop.push_back(int(info.size()));
		//存入文件路径
		this->_filePathGroup.push_back(url);
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

std::vector<QString>::iterator ManagerMent::GetLastFilesPathGroup()
{
	//判断是否有未处理的路径
	if (!this->_filePathGroup.empty())
	{
		return --this->_filePathGroup.end(); //--回到最后一个元素
	}
	else
	{
		//已经到底了，指向空
		return this->_filePathGroup.end();
	}

	return this->_filePathGroup.end();
}

std::vector<int>::iterator ManagerMent::GetLastFilesSizeGroup()
{
	//判断是否有未处理的路径
	if (!this->_filePathGroup.empty())
	{
		return --this->_filesSizeGruop.end();
	}
	else
	{
		//已经到底了，指向空
		return this->_filesSizeGruop.end();
	}

	return this->_filesSizeGruop.end();
}

vector<QString>& ManagerMent::GetBackSuffix()
{
	//返回后缀库
	return this->_backSuffix;
}

void ManagerMent::DeleteLastFiles()
{
	//删除最后一个文件的所有信息
	this->_filePathGroup.pop_back();//删除路径
	this->_filesSizeGruop.pop_back();//删除大小
}

int ManagerMent::GetNowFilesNum()
{
	return _filePathGroup.size();
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

void ManagerMent::PrintAllFilesInfo()
{
	//打印所有文件数据
	vector<QString>::iterator qstrIt = this->GetLastFilesPathGroup();
	vector<int>::iterator intIt = this->GetLastFilesSizeGroup();

	for (int i = this->GetNowFilesNum(); i > 0; i--)
	{
		qDebug() << *qstrIt << "    " << *intIt << '\n';
		if (qstrIt != this->_filePathGroup.begin() && intIt != this->_filesSizeGruop.begin())
		{
			qstrIt--;
			intIt--;
		}
	}
}