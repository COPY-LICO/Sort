#include "HistoryInfo.h"

HistoryInfo::HistoryInfo(QWidget* parent) : QDialog(parent)
{
	dialogui.setupUi(this);

	//设置主Widget布局
	QVBoxLayout* layout = new QVBoxLayout(dialogui.Dmain_Widget);
	

}

HistoryInfo::~HistoryInfo()
{
}