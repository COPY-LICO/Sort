	#pragma once

const int chooseNull = -1;

enum ChooseForm
{
	Sort,
	Rename
};

enum SortType
{
	byTimePoints,
	byFileTypes,
	byFileSize,
};

enum RenameType
{
	renamePrefix,
	renameSuffix,	
	renameByKeyWord
};

struct InfoGroup
{
public:
	//默认值为-1，表示未选中
	int chooseForm = chooseNull;
	int sortType = chooseNull;
	int renameType = chooseNull;
};

struct DetailInfo
{
public:
	//时间点方案
	bool byYear = false;
	bool byYear_Month = false;
	//类型方案
	std::vector<QString> typeGroup = {};
	//大小方案
	int largeFile = chooseNull;
	int smallFile = chooseNull;

	//重命名方案
	QString renameContent = "";
};