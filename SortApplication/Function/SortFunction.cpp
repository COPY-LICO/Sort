#include "SortFunction.h"
#include "ManagerMent.h"
#include "InfoGroup.h"
#include <qdir.h>
#include <qmessagebox.h>
#include <qdebug.h>  
#include <QFileInfo>
#include <QFileDialog>

SortFunction::SortFunction(QWidget* parent) : QObject(parent)
{
    //获取操作管理对象
    this->manager = ManagerMent::GetInstance();
}

//槽函数 -- -- 
//根据ManagerMent的信息来决定采用的分类方法
bool SortFunction::SureSortOperator()
{
    // 清空上一次的操作记录
    manager->ClearAllRecordFiles();

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
    }
    else if (infoType->chooseForm == ChooseForm::Rename) //采用重命名
    {
        if (infoType->renameType == RenameType::renamePrefix)
        {
            //获取信息 - 调用添加前缀函数
            return this->RenameFileByPrefix();
        }
        else if (infoType->renameType == RenameType::renameSuffix)
        {
            //获取信息 - 调用修改后缀函数
            return this->RenameFileBySuffix();
        }
        else if (infoType->renameType == RenameType::renameByKeyWord)
        {
            //获取信息 - 调用统一名称函数
            return this->RenameFileByKeyWord();
        }
    }

    return false;
}

//撤回函数
bool SortFunction::WithDrawOperator()
{
    // 获取历史记录迭代器
    std::vector<RecordFiles>::iterator recordIt = manager->GetRecordFilesGroup();
    if (manager->IsRecordFilesEmpty()) 
    {
        QMessageBox::warning(nullptr, "提示", "暂无可撤回的操作！");
        return false;
    }

    manager->PrintAllRecordFilesInfo();

    // 遍历所有记录，逐个恢复文件
    int recordNum = manager->GetRecordFilesNum(); 
    for (int i = 0; i < recordNum; i++)
    {
        QString sourcePath = recordIt->newFilePath;
        QString movePath = recordIt->oldFilePath;
        if (!QFile::rename(sourcePath, movePath))
        {
            QMessageBox::warning(nullptr, "失败", "文件移动失败" + sourcePath);
        }

        if (!(manager->IsRecordFileItPosFilesTop(recordIt)))
        {
            recordIt--;
        }
    }

    // 清空撤回记录
    manager->ClearAllRecordFiles();
    QMessageBox::information(nullptr, "成功", "撤回操作完成！");
    manager->IndexDecrement();
    return true;
}

//槽函数 -- --

// //时间区间分类
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
    if (!&(*fileIt))
    {
        QMessageBox::warning(nullptr, "错误", "文件列表指针为空！");
        return false;
    }

    // 获取用户指定的分类结果路径
    QString userSpecifiedPath = manager->GetMovePath();
    if (userSpecifiedPath.isEmpty())
    {
        // 如果未设置路径，弹出对话框让用户选择
        userSpecifiedPath = QFileDialog::getExistingDirectory(nullptr, "选择分类结果保存路径", QDir::homePath());
        if (userSpecifiedPath.isEmpty())
        {
            QMessageBox::warning(nullptr, "错误", "未指定分类结果保存路径！");
            return false;
        }
        // 保存用户选择的路径到manager
        manager->SaveMovePath(userSpecifiedPath);
    }

    // 确保用户指定的路径存在
    QDir userDir(userSpecifiedPath);
    if (!userDir.exists())
    {
        // 不存在则创建
        if (!userDir.mkpath(userSpecifiedPath))
        {
            QMessageBox::warning(nullptr, "错误", "创建用户指定路径失败！");
            return false;
        }
    }

    //  逐个文件重命名分类
    for (int i = 0; i < fileNum; i++)
    {
        Files& file = *fileIt;
        QString year = file.modifyTime.left(4);
        QString month = file.modifyTime.mid(5, 2);

        // 确定重命名
        QString timeTag;
        if (rule->byYear && rule->byYear_Month)
        {
            timeTag = year + "_" + month;
        }
        else if (rule->byYear)
        {
            timeTag =year;
        }
        else if (rule->byYear_Month)
        {
            timeTag =  month ;
        }
        else
        {
            QMessageBox::warning(nullptr, "错误", "未选择按年/按月！");
            return false;
        }

        // 文件夹创建到用户指定路径
        QDir folder;
        QString folderName = "sort_by_time_" + timeTag;
        QString folderPath = userSpecifiedPath + "/" + folderName;
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

        // 保存撤回记录
        manager->SaveRecordFiles(file.fileName, file.fileName, oldPath, newPath);

        // 更新原数据
        file.filePath = newPath;

        if (i != fileNum - 1)
        {
            fileIt--;
        }
    }

    QMessageBox::information(nullptr, "成功", "按时间区间分类完成！");
    //发送结束信号
    manager->EndOperator();
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
    if (!&(*fileIt))
    {
        QMessageBox::warning(nullptr, "错误", "文件列表指针为空！");
        return false;
    }

    // 获取用户指定的分类结果路径
    QString userSpecifiedPath = manager->GetMovePath();
    if (userSpecifiedPath.isEmpty())
    {
        // 弹出对话框让用户选择路径
        userSpecifiedPath = QFileDialog::getExistingDirectory(nullptr, "选择分类结果保存路径", QDir::homePath());
        if (userSpecifiedPath.isEmpty())
        {
            QMessageBox::warning(nullptr, "错误", "未指定分类结果保存路径！");
            return false;
        }
        manager->SaveMovePath(userSpecifiedPath);
    }

    // 确保用户指定的路径存在
    QDir userDir(userSpecifiedPath);
    if (!userDir.exists())
    {
        if (!userDir.mkpath(userSpecifiedPath))
        {
            QMessageBox::warning(nullptr, "错误", "创建用户指定路径失败！");
            return false;
        }
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
        QDir folder;
        QString folderName = "sort_by_type_" + suffix;
        QString folderPath = userSpecifiedPath + "/" + folderName;
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

        // 保存撤回记录
        manager->SaveRecordFiles(file.fileName, file.fileName, oldPath, newPath);

        // 更新原数据
        if (i != fileNum - 1)
        {
            fileIt--;
        }
    }

    QMessageBox::information(nullptr, "成功", "按文件类型分类完成！");
    //发送结束信号
    manager->EndOperator();
    return true;
}

//文件大小分类
bool SortFunction::SortFileByFileSize()
{
    // 获取分类规则
    DetailInfo* rule = manager->GetOperatorContent();
    if (!rule)
    {
        QMessageBox::warning(nullptr, "错误", "获取大小分类规则失败！");
        return false;
    }

    int largeFile = rule->largeFile;
    int smallFile = rule->smallFile;

    // 校验参数
    if (largeFile < 0 && smallFile < 0)
    {
        QMessageBox::warning(nullptr, "错误", "文件大小参数设置错误！");
        return false;
    }

    // 获取文件总数
    int fileNum = manager->GetNowFilesNum();
    if (fileNum <= 0)
    {
        QMessageBox::warning(nullptr, "提示", "暂无文件可分类！");
        return false;
    }

    std::vector<Files>::iterator fileIt = manager->GetLastFilesPathGroup();
    if (!&(*fileIt))
    {
        QMessageBox::warning(nullptr, "错误", "文件列表指针为空！");
        return false;
    }

    // 获取用户指定的分类结果路径
    QString userSpecifiedPath = manager->GetMovePath();
    if (userSpecifiedPath.isEmpty())
    {
        // 弹出对话框让用户选择路径
        userSpecifiedPath = QFileDialog::getExistingDirectory(nullptr, "选择分类结果保存路径", QDir::homePath());
        if (userSpecifiedPath.isEmpty())
        {
            QMessageBox::warning(nullptr, "错误", "未指定分类结果保存路径！");
            return false;
        }
        manager->SaveMovePath(userSpecifiedPath);
    }

    // 确保用户指定的路径存在
    QDir userDir(userSpecifiedPath);
    if (!userDir.exists())
    {
        if (!userDir.mkpath(userSpecifiedPath))
        {
            QMessageBox::warning(nullptr, "错误", "创建用户指定路径失败！");
            return false;
        }
    }

    // 逐个文件分类
    for (int i = 0; i < fileNum; i++)
    {
        Files& file = *fileIt;
        int fileSize = file.size;
        int fileSizeKB = fileSize / 1024;

        QDir folder;
        QString folderName;

        // 判断文件大小分类
        if (smallFile >= 0 && largeFile >= 0)
        {
            if (fileSizeKB <= smallFile)
                folderName = "sort_by_size_small";
            else if (fileSizeKB >= largeFile)
                folderName = "sort_by_size_large";
            else
                folderName = "sort_by_size_mid";
        }
        else if (smallFile >= 0)
        {
            folderName = fileSizeKB <= smallFile ? "sort_by_size_small" : "sort_by_size_large";
        }
        else if (largeFile >= 0)
        {
            folderName = fileSizeKB >= largeFile ? "sort_by_size_large" : "sort_by_size_small";
        }

        // 文件夹创建到用户指定路径
        QString folderPath = userSpecifiedPath + "/" + folderName;
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

        // 保存撤回记录
        manager->SaveRecordFiles(file.fileName, file.fileName, oldPath, newPath);
       // qDebug() << file.fileName << '\n' << oldPath << '\n' << newPath;

        // 更新原数据
        file.filePath = newPath;

        if (i != fileNum - 1)
        {
            fileIt--;
        }
    }

    QMessageBox::information(nullptr, "成功", "按文件大小分类完成！");
    //发送结束信号
    manager->EndOperator();
    return true;
}
//添加前缀重命名
bool SortFunction::RenameFileByPrefix()
{
    // 获取重命名规则
    DetailInfo* rule = manager->GetOperatorContent();
    if (!rule)
    {
        QMessageBox::warning(nullptr, "错误", "获取重命名规则失败！");
        return false;
    }

    QString prefix = rule->renameContent;
    if (prefix.isEmpty())
    {
        QMessageBox::warning(nullptr, "提示", "前缀内容不能为空！");
        return false;
    }

    // 获取文件总数
    int fileNum = manager->GetNowFilesNum();
    if (fileNum <= 0)
    {
        QMessageBox::warning(nullptr, "提示", "暂无文件可重命名！");
        return false;
    }

    std::vector<Files>::iterator fileIt = manager->GetLastFilesPathGroup();
    if (!&(*fileIt))
    {
        QMessageBox::warning(nullptr, "错误", "文件列表指针为空！");
        return false;
    }

    // 逐个文件添加前缀
    for (int i = 0; i < fileNum; i++)
    {
        Files& file = *fileIt;
        QFileInfo fileInfo(file.filePath);
        QString fileDir = fileInfo.path();
        QString oldFileName = file.fileName;
        QString newFileName = prefix + "_" + oldFileName;
        QString newFilePath = fileDir + "/" + newFileName;

        // 处理重名
        int num = 1;
        while (QFile::exists(newFilePath))
        {
            newFileName = prefix + "_" + file.prefix + "_" + QString::number(num) + "." + file.suffix;
            newFilePath = fileDir + "/" + newFileName;
            num++;
        }

        // 执行重命名
        if (!QFile::rename(file.filePath, newFilePath))
        {
            QMessageBox::warning(nullptr, "错误", "文件重命名失败：" + file.fileName);
            return false;
        }

        // 保存撤回记录
        manager->SaveRecordFiles(oldFileName, newFileName, file.filePath, newFilePath);

        // 更新原数据
        file.filePath = newFilePath;
        file.fileName = newFileName;

        if (i != fileNum - 1)
        {
            fileIt--;
        }
    }

    QMessageBox::information(nullptr, "成功", "添加前缀重命名完成！");
    //发送结束信号
    manager->EndOperator();
    return true;
}

//统一修改后缀重命名
bool SortFunction::RenameFileBySuffix()
{
    // 获取重命名规则
    DetailInfo* rule = manager->GetOperatorContent();
    if (!rule)
    {
        QMessageBox::warning(nullptr, "错误", "获取重命名规则失败！");
        return false;
    }

    QString newSuffix = rule->renameContent;
    if (newSuffix.isEmpty())
    {
        QMessageBox::warning(nullptr, "提示", "后缀内容不能为空！");
        return false;
    }
    // 统一后缀格式
    if (newSuffix.startsWith("."))
    {
        newSuffix = newSuffix.mid(1);
    }

    // 获取文件总数
    int fileNum = manager->GetNowFilesNum();
    if (fileNum <= 0)
    {
        QMessageBox::warning(nullptr, "提示", "暂无文件可重命名！");
        return false;
    }

    std::vector<Files>::iterator fileIt = manager->GetLastFilesPathGroup();
    if (!&(*fileIt))
    {
        QMessageBox::warning(nullptr, "错误", "文件列表指针为空！");
        return false;
    }

    // 逐个文件修改后缀
    for (int i = 0; i < fileNum; i++)
    {
        Files& file = *fileIt;
        QFileInfo fileInfo(file.filePath);
        QString fileDir = fileInfo.path();
        QString oldFileName = file.fileName;
        // 新文件名：原前缀 + 统一后缀
        QString newFileName = file.prefix + "." + newSuffix;
        QString newFilePath = fileDir + "/" + newFileName;

        // 处理重名
        int num = 1;
        while (QFile::exists(newFilePath))
        {
            newFileName = file.prefix + "_" + QString::number(num) + "." + newSuffix;
            newFilePath = fileDir + "/" + newFileName;
            num++;
        }

        // 执行重命名
        if (!QFile::rename(file.filePath, newFilePath))
        {
            QMessageBox::warning(nullptr, "错误", "文件重命名失败：" + file.fileName);
            return false;
        }

        // 保存撤回记录
        manager->SaveRecordFiles(oldFileName, newFileName, file.filePath, newFilePath);

        // 更新原数据
        file.filePath = newFilePath;
        file.fileName = newFileName;
        file.suffix = newSuffix;

        if (i != fileNum - 1)
        {
            fileIt--;
        }
    }

    QMessageBox::information(nullptr, "成功", "统一修改后缀完成！");
    //发送结束信号
    manager->EndOperator();
    return true;
}

//统一名称重命名
bool SortFunction::RenameFileByKeyWord()
{
    // 获取重命名规则
    DetailInfo* rule = manager->GetOperatorContent();
    if (!rule)
    {
        QMessageBox::warning(nullptr, "错误", "获取重命名规则失败！");
        return false;
    }

    QString keyWord = rule->renameContent;
    if (keyWord.isEmpty())
    {
        QMessageBox::warning(nullptr, "提示", "统一名称不能为空！");
        return false;
    }

    // 获取文件总数
    int fileNum = manager->GetNowFilesNum();
    if (fileNum <= 0)
    {
        QMessageBox::warning(nullptr, "提示", "暂无文件可重命名！");
        return false;
    }

    std::vector<Files>::iterator fileIt = manager->GetLastFilesPathGroup();
    if (!&(*fileIt))
    {
        QMessageBox::warning(nullptr, "错误", "文件列表指针为空！");
        return false;
    }

    // 逐个文件统一命名
    for (int i = 0; i < fileNum; i++)
    {
        Files& file = *fileIt;
        QFileInfo fileInfo(file.filePath);
        QString fileDir = fileInfo.path();
        QString oldFileName = file.fileName;
        // 新文件名：统一关键词 + 序号 + 原后缀
        QString newFileName = keyWord + "_" + QString::number(i + 1) + "." + file.suffix;
        QString newFilePath = fileDir + "/" + newFileName;

        // 处理重名
        int num = 1;
        while (QFile::exists(newFilePath))
        {
            newFileName = keyWord + "_" + QString::number(i + 1) + "_" + QString::number(num) + "." + file.suffix;
            newFilePath = fileDir + "/" + newFileName;
            num++;
        }

        // 执行重命名
        if (!QFile::rename(file.filePath, newFilePath))
        {
            QMessageBox::warning(nullptr, "错误", "文件重命名失败：" + file.fileName);
            return false;
        }

        // 保存撤回记录
        manager->SaveRecordFiles(oldFileName, newFileName, file.filePath, newFilePath);

        // 更新原数据
        file.filePath = newFilePath;
        file.fileName = newFileName;
        file.prefix = keyWord + "_" + QString::number(i + 1);

        if (i != fileNum - 1)
        {
            fileIt--;
        }
    }

    QMessageBox::information(nullptr, "成功", "统一名称重命名完成！");
    //发送结束信号
    manager->EndOperator();
    return true;
}