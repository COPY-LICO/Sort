#include "HistoryInfo.h"
#include <QFileIconProvider>
#include <qlabel.h>

HistoryInfo::HistoryInfo(QWidget* parent) : QDialog(parent)
{
	dialogui.setupUi(this);

	ManagerMent* _manager = ManagerMent::GetInstance();

	//设置主Widget布局
	QVBoxLayout* layout = new QVBoxLayout(dialogui.Dmain_Widget);
	

	/*========HistoryListWidget========*/

	//隐藏滚动条
	dialogui.DoriginalFiles_listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	dialogui.DoriginalFiles_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //获取历史文件信息
    AddOriginalFiles(_manager->ReturnOperationAllFileRecord());

}

//将原文件置入列表
void HistoryInfo::AddOriginalFiles(std::vector<IntegratedContent>HistoryFiles)
{
	//获取实例
	ManagerMent* _manager = ManagerMent::GetInstance();

    if (HistoryFiles.empty())
    {
        qDebug() << "Failed!";
        return;
    }

    //清空列表
    dialogui.DoriginalFiles_listWidget->clear();


    //创建自定义Widget来显示内容
    QWidget* fileWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(fileWidget);  //水平排布
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(12);
    fileWidget->setStyleSheet("background-color: transparent; border: none;");


	for (const auto& content : HistoryFiles) {
		//创建列表项
		QListWidgetItem* item = new QListWidgetItem(dialogui.DoriginalFiles_listWidget);
		item->setSizeHint(QSize(0, 108));

		//创建自定义Widget
		QWidget* fileWidget = new QWidget();
		QVBoxLayout* widgetLayout = new QVBoxLayout(fileWidget);
		widgetLayout->setContentsMargins(8, 8, 8, 8);
		widgetLayout->setSpacing(4); // 缩小内部间距
		fileWidget->setStyleSheet("background-color: transparent; border: none;");

		//提取并展示旧文件名
		QLabel* lblOldName = new QLabel(fileWidget);
		lblOldName->setText(QString("OldName：%1 ->").arg(content.oldFileName));
		lblOldName->setStyleSheet("font-size: 12px; color: #333333;");
		lblOldName->setWordWrap(true); // 超长名称自动换行

		//新文件名
		QLabel* lblNewName = new QLabel(fileWidget);
		lblNewName->setText(QString("NewName：%1").arg(content.newFileName));
		lblNewName->setStyleSheet("font-size: 12px; color: #333333;");
		lblNewName->setWordWrap(true); // 超长名称自动换行

		//提取并展示旧文件路径
		QLabel* lblOldPath = new QLabel(fileWidget);
		lblOldPath->setText(QString("OldPath：%1 ->").arg(content.oldFilePath));
		lblOldPath->setStyleSheet("font-size: 11px; color: #666666;");
		lblOldPath->setWordWrap(true);

		//新文件路径
		QLabel* lblNewPath = new QLabel(fileWidget);
		lblNewPath->setText(QString("NewPath：%1").arg(content.oldFilePath));
		lblNewPath->setStyleSheet("font-size: 11px; color: #666666;");
		lblNewPath->setWordWrap(true);

		//提取并展示文件大小
		QLabel* lblSize = new QLabel(fileWidget);
		lblSize->setText(QString("Size：%1").arg(content.size));
		lblSize->setStyleSheet("font-size: 11px; color: #999999;");

		//操作类型
		QString operationText = QString("%1").arg(content.operationType);
		QLabel* operationLabel = new QLabel(operationText);
		operationLabel->setStyleSheet("font-weight: bold; color: #333333; font-size: 14px;");

		//修改关键词
		QString modeText = QString("OperMode：%1").arg(content.modifyContent);
		QLabel* modeLabel = new QLabel(modeText);
		modeLabel->setStyleSheet("font-size: 12px; color: #666666;");


		//将标签加入布局
		widgetLayout->addWidget(lblOldName);
		widgetLayout->addWidget(lblNewName);
		widgetLayout->addWidget(lblOldPath);
		widgetLayout->addWidget(lblNewPath);
		widgetLayout->addWidget(lblSize);
		widgetLayout->addWidget(operationLabel);
		widgetLayout->addWidget(modeLabel);

		//绑定Widget到列表项
		dialogui.DoriginalFiles_listWidget->setItemWidget(item, fileWidget);
	}


    // 核心：仅保留一层淡灰外边框，选中无任何额外框
    dialogui.DoriginalFiles_listWidget->setStyleSheet(R"(
    /* 列表容器：无边框、透明背景，仅控制item间距 */
    QListWidget {
        border: none;
        background-color: transparent;
    }

    /* item默认状态 */
    QListWidget::item {
        background-color: #FFFFFF;
        border: 1px solid rgb(0, 170, 255); 
        border-radius: 6px; 

    }

    /* 悬停状态 */
    QListWidget::item:hover {
        background-color: rgb(245,245,245);
    }

    /* 选中状态 */
    QListWidget::item:selected,
    QListWidget::item:selected:focus {
        background-color: rgb(245,245,245); 
        border: 1px solid rgb(0, 170, 255); 
        outline: none; /* 去掉选中虚线框 */

    }
)");
    //为item之间设置间距
    dialogui.DoriginalFiles_listWidget->setSpacing(8);
    // 保证item可选中，但无焦点虚线框（平衡交互和样式）
    dialogui.DoriginalFiles_listWidget->setFocusPolicy(Qt::NoFocus);

   
}

HistoryInfo::~HistoryInfo()
{
}