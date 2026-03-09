#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HistoryInfo.h"
#include "SortApplication.h"
#include "ManagerMent.h"
#include <QFileDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>

class HistoryInfo : public QDialog
{
	Q_OBJECT

public:
	HistoryInfo(int historyIndex, QWidget* parent = nullptr);

	//将原文件置入列表
	void AddOriginalFiles(std::vector<IntegratedContent>HistoryFiles);

	~HistoryInfo();

private:
	Ui::Dialog dialogui;
	//历史记录索引
	int _historyIndex;

};