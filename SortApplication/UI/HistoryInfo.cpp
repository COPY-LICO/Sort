#include "HistoryInfo.h"

HistoryInfo::HistoryInfo(QWidget* parent) : QDialog(parent)
{
	setWindowTitle("HistoryInfo");
	resize(400, 300); // 窗口大小

	QVBoxLayout* layout = new QVBoxLayout(this);
	QLabel* tipLabel = new QLabel("HistoryInfo", this);
	tipLabel->setAlignment(Qt::AlignCenter);
	layout->addWidget(tipLabel);
}

HistoryInfo::~HistoryInfo()
{
}