#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SortApplication.h"
#include <QFileDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>

class HistoryInfo : public QDialog
{
	Q_OBJECT

public:
	HistoryInfo(QWidget* parent = nullptr);


	~HistoryInfo();

};