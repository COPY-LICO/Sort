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