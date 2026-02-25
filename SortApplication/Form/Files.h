#pragma once
#include<qstring.h>
struct Files
{
public:

	QString prefix;
	QString suffix;
	QString modifyTime;
	QString filePath;
	QString fileName;
	int size;
};

struct RecordFiles
{
public:
	QString oldFileName;
	QString newFileName;
	QString oldFilePath;
	QString newFilePath;
};