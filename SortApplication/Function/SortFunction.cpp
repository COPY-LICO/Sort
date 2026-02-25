#include "SortFunction.h"
#include "ManagerMent.h"
#include "InfoGroup.h"
#include <qdir.h>
#include <qmessagebox.h>
#include <qdebug.h>  
#include <QFileInfo>
#include <QDateTime>

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

//根据ManagerMent的信息来决定采用的分类方法
bool SortFunction::SureSortOperator()
{
    InfoGroup* infoType = manager->GetOperatorType();
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
        QFileInfo fileInfo(file.filePath);
        QString fileDir = fileInfo.path();

        // 获取文件修改时间
        QDateTime modifyTime = fileInfo.lastModified();
        QString timeTag = modifyTime.toString("yyyyMMdd_HHmmss"); 

        QString newFileName = file.prefix + "_" + timeTag + "." + file.suffix;
        if (file.suffix.isEmpty())
        {
            newFileName = file.prefix + "_" + timeTag; 
        }
        QString newFilePath = fileDir + "/" + newFileName;

        // 处理重名
        int num = 1;
        while (QFile::exists(newFilePath))
        {
            newFileName = file.prefix + "_" + timeTag + "_" + QString::number(num) + "." + file.suffix;
            if (file.suffix.isEmpty()) {
                newFileName = file.prefix + "_" + timeTag + "_" + QString::number(num);
            }
            newFilePath = fileDir + "/" + newFileName;
            num++;
        }

        // 执行重命名
        if (!QFile::rename(file.filePath, newFilePath)) {
            QMessageBox::warning(nullptr, "错误", "文件按时间分类失败：" + file.fileName);
            return false;
        }
    }

    QMessageBox::information(nullptr, "成功", "按时间区间分类完成！");
    return true;
}

//文件类型分类
bool SortFunction::SortFileByFileType()
{
    DetailInfo* rule = manager->GetOperatorContent();
    if (!rule)
    {
        QMessageBox::warning(nullptr, "错误", "获取类型分类规则失败！");
        return false;
    }

    std::vector<QString> typeList = rule->typeGroup;
    if (typeList.empty())
    {
        QMessageBox::warning(nullptr, "提示", "未选择要分类的文件类型！");
        return false;
    }

    std::vector<Files> fileList = GetFileList(manager);
    if (fileList.empty())
    {
        QMessageBox::warning(nullptr, "提示", "暂无文件可分类！");
        return false;
    }

    // 逐个文件：加类型标签重命名
    for (int i = 0; i < fileList.size(); i++)
    {
        Files file = fileList[i];
        QString suffix = file.suffix;
        if (suffix.isEmpty())
        {
            suffix = "no_suffix";
        }

        // 判断是否是目标类型
        bool isTarget = false;
        for (int j = 0; j < typeList.size(); j++)
        {
            QString type = typeList[j];
            if (type == suffix || type == "." + suffix)
            {
                isTarget = true;
                break;
            }
        }
        if (!isTarget)
            continue;

        QFileInfo fileInfo(file.filePath);
        QString fileDir = fileInfo.path();

        QString newFileName = file.prefix + "_type_" + suffix + "." + file.suffix;
        if (file.suffix.isEmpty())
        {
            newFileName = file.prefix + "_type_" + suffix;
        }
        QString newFilePath = fileDir + "/" + newFileName;

        // 处理重名
        int num = 1;
        while (QFile::exists(newFilePath))
        {
            newFileName = file.prefix + "_type_" + suffix + "_" + QString::number(num) + "." + file.suffix;
            if (file.suffix.isEmpty()) {
                newFileName = file.prefix + "_type_" + suffix + "_" + QString::number(num);
            }
            newFilePath = fileDir + "/" + newFileName;
            num++;
        }

        // 执行重命名
        if (!QFile::rename(file.filePath, newFilePath))
        {
            QMessageBox::warning(nullptr, "错误", "文件按类型分类失败：" + file.fileName);
            return false;
        }
    }

    QMessageBox::information(nullptr, "成功", "按文件类型分类完成！");
    return true;
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