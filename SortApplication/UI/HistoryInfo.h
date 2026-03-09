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

	//��ԭ�ļ������б�
	void AddOriginalFiles(std::vector<IntegratedContent>HistoryFiles);

	~HistoryInfo();

private:
	Ui::Dialog dialogui;
	//��ʷ��¼����
	int _historyIndex;

};