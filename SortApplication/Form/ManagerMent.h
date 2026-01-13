#pragma once
#include <string>
//这是一个全局管理类，里面所有函数都采用静态形式，可在全局调用负责启动API接口

using std::string;

class ManagerMent
{
public:
	//ui存入获取到的文件路径
	static bool SaveTheFilePath(string url);
	//获取存放的文件路径
	static string GetFilePath();
private:
	static string filePath;
};

