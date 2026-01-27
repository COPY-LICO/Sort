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
	bySameFileName

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
	//时间分类 - 筛选区间
	QString startTime;
	QString endTime;
	//类型分类 - 筛选类型
	QString suffixDetail;//单类型
	//大小分类 - 划分大小
	int midsize;
	//名称分类 - 筛选名称内的特定信息
	QString nameContentDetail;
};