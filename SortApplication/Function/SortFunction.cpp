#include "SortFunction.h"
#include "ManagerMent.h"
#include "InfoGroup.h"
#include <qdir.h>
#include <qmessagebox.h>
#include <qdebug.h>  
#include <QFileInfo>
#include <QDateTime>

SortFunction::SortFunction(QWidget* parent) : QObject(parent)
{
    //获取操作管理对象
    this->manager = ManagerMent::GetInstance();
}

//槽函数 -- -- 
//根据ManagerMent的信息来决定采用的分类方法
bool SortFunction::SureSortOperator()
{
    InfoGroup* infoType = manager->GetOperatorType();
    if (infoType->chooseForm == ChooseForm::Sort) //采用分类
    {
        if (infoType->sortType == SortType::byTimePoints) // 根据时间分类
        {
            //获取信息 - 调用时间分类函数
            return this->SortFileByTimePoint();
        }
        else if (infoType->sortType == SortType::byFileTypes) // 根据文件类型分类
        {
            //获取信息 - 调用类型分类函数
            return this->SortFileByFileType();
        }
        else if (infoType->sortType == SortType::byFileSize) // 根据文件大小分类
        {
            //获取信息 - 调用大小分类函数
            return this->SortFileByFileSize();
        }
        else if (infoType->sortType == SortType::bySameFileName) // 根据文件名分类
        {
            //获取信息 - 调用文件名分类函数
            return this->SortFileByFileName();
        }
    }
    else if (infoType->chooseForm == ChooseForm::Rename) //采用重命名
    {
        //未开发
        return false;
    }

    return false;
}
//时间区间分类
bool SortFunction::SortFileByTimePoint()
{
    //  获取分类规则
    DetailInfo* rule = manager->GetOperatorContent();
    if (!rule)
    {
        QMessageBox::warning(nullptr, "错误", "获取时间分类规则失败！");
        return false;
    }

    //  获取文件总数
    int fileNum = manager->GetNowFilesNum();
    if (fileNum <= 0)
    {
        QMessageBox::warning(nullptr, "提示", "暂无文件可分类！");
        return false;
    }

    std::vector<Files>::iterator fileIt = manager->GetLastFilesPathGroup();
    if (!fileIt._Ptr) 
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

        file.filePath = newFilePath;
        file.fileName = newFileName;

        if (i != fileNum - 1)
        {
            fileIt--;
        }
    }

    QMessageBox::information(nullptr, "成功", "按时间区间分类完成！");
    return true;
}

//文件类型分类
bool SortFunction::SortFileByFileType()
{
    //  获取分类规则
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

    //  获取文件总数
    int fileNum = manager->GetNowFilesNum();
    if (fileNum <= 0)
    {
        QMessageBox::warning(nullptr, "提示", "暂无文件可分类！");
        return false;
    }

    std::vector<Files>::iterator fileIt = manager->GetLastFilesPathGroup();
    if (!fileIt._Ptr)
    {
        QMessageBox::warning(nullptr, "错误", "文件列表指针为空！");
        return false;
    }

    //  逐个文件分类
    for (int i = 0; i < fileNum; i++)
    {
        Files& file = *fileIt; 
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
        {
            if (i != fileNum - 1)
            {
                fileIt--;
            }
            continue;
        }

        // 原文件目录创建类型文件夹
        QFileInfo fileInfo(file.filePath);
        QString fileDir = fileInfo.path();
        QDir folder;
        QString folderName = "sort_by_type_" + suffix;
        QString folderPath = fileDir + "/" + folderName;
        if (!folder.exists(folderPath))
        {
            folder.mkdir(folderPath);
        }

        // 移动文件
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

        if (i != fileNum - 1)
        {
            fileIt--;
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