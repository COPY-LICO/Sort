#include "SortFunction.h"
#include "ManagerMent.h"
#include "InfoGroup.h"
#include <qdir.h>
#include <qmessagebox.h>
#include <qdebug.h>

// 从ManagerMent获取文件列表
std::vector<Files> GetFileList(ManagerMent* manager)
{
    std::vector<Files> fileList;
    int fileNum = manager->GetNowFilesNum();
    if (fileNum <= 0)
    {
        return fileList;
    }

    auto fileIt = manager->GetLastFilesPathGroup();
    // 所有文件存入列表
    for (int i = 0; i < fileNum; i++)
    {
        fileList.push_back(*fileIt);
        if (i != fileNum - 1)
        {
            fileIt--; 
        }
    }
    return fileList;
}

SortFunction::SortFunction(QWidget* parent = nullptr) : QObject(parent)
{
    //获取操作管理对象
    this->manager = ManagerMent::GetInstance();
}

//槽函数 -- -- 
//根据ManagerMent的信息来决定采用的分类方法
bool SortFunction::SureSortOperator()
{
    InfoGroup* infoType = manager->GetOperatorType();
    this->manager->ClearAllRecordFiles();

    if (infoType->chooseForm == 0) //采用分类
    {
        if (infoType->sortType == 0) // 根据时间分类
        {
            //获取信息 - 调用时间分类函数
            return this->SortFileByTimePoint();
        }
        else if (infoType->sortType == 1) // 根据文件类型分类
        {
            //获取信息 - 调用类型分类函数
            return this->SortFileByFileType();
        }
        else if (infoType->sortType == 2) // 根据文件大小分类
        {
            //获取信息 - 调用大小分类函数
            return this->SortFileByFileSize();
        }
        else if (infoType->sortType == 3) // 根据文件名分类
        {
            //获取信息 - 调用文件名分类函数
            return this->SortFileByFileName();
        }
    }
    else if (infoType->chooseForm == 1) //采用重命名
    {
        //未开发
        return false;
    }

    return false;
}

//撤回函数内容 - 重命名和分类撤回方式一致
bool SortFunction::WithDrawOperator()
{
    return false; // 未开发完成
}

//槽函数 -- --

//时间区间分类
bool SortFunction::SortFileByTimePoint()
{
    //  获取分类规则（按年/按月）
    DetailInfo* rule = manager->GetOperatorContent();
    if (!rule) 
    {
        QMessageBox::warning(nullptr, "错误", "获取时间分类规则失败！");
        return false;
    }

    //  获取所有文件列表
    std::vector<Files> fileList = GetFileList(manager);
    if (fileList.empty()) 
    {
        QMessageBox::warning(nullptr, "提示", "暂无文件可分类！");
        return false;
    }

    //  逐个文件分类
    for (int i = 0; i < fileList.size(); i++)
    {
        Files file = fileList[i]; 
        QString year = file.modifyTime.left(4);
        QString month = file.modifyTime.mid(5, 2);

        // 确定文件夹名称
        QString folderName;
        if (rule->byYear && rule->byYear_Month)
        {
            folderName = "按时间分类_" + year + "年" + month + "月";
        }
        else if (rule->byYear) 
        {
            folderName = "按时间分类_" + year + "年";
        }
        else if (rule->byYear_Month)
        {
            folderName = "按时间分类_" + month + "月";
        }
        else 
        {
            QMessageBox::warning(nullptr, "错误", "未选择按年/按月！");
            return false;
        }

        // 创建文件夹
        QDir folder;
        QString folderPath = QDir::currentPath() + "/" + folderName;
        if (!folder.exists(folderPath)) 
        {
            folder.mkdir(folderPath);
        }

        // 移动文件（处理重名）
        QString oldPath = file.filePath;
        QString newPath = folderPath + "/" + file.fileName;
        int num = 1;
        while (QFile::exists(newPath))
        {
            newPath = folderPath + "/" + file.prefix + "_" + QString::number(num) + "." + file.suffix;
            num++;
        }

        // 执行移动
        if (!QFile::rename(oldPath, newPath)) 
        {
            QMessageBox::warning(nullptr, "错误", "文件移动失败：" + file.fileName);
            return false;
        }

        this->manager->SaveRecordFiles("","","","");
    }

    QMessageBox::information(nullptr, "成功", "按时间分类完成！");
    return true;
}

//文件类型分类
bool SortFunction::SortFileByFileType()
{
    return false;
}

//文件大小分类
bool SortFunction::SortFileByFileSize()
{
    return false;
}

//文件名相同筛选分类
bool SortFunction::SortFileByFileName()
{
    return false;
}