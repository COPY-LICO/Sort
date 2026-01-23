# 接口使用

**ManagerMent方法**

`static ManagerMent* GetInstance()`来获取对应的ManagerMent的指针对象，方便调用ManagerMent的相关操作  
eg: `ManagerMent* manger = ManagerMent::GetInstance();`

`void SaveFile(QString)`来存入获取到的单个文件(只需要文件路径)，后续文件处理等操作由ManagerMent完成  
eg: `manger->SaveFile(url);`  

`vector<Files>::iterator GetLastFilesPathGroup();`来获取存入文件的路径库的最后一个元素迭代器  
eg: `vector<Files>::iterator url = manger->GetFilesPathGroup();`  

`int GetNowFilesNum();`来获取当前存入的文件数目  
eg: `int number = manger->GetNowFilesNum();`  

`void DeleteLastFiles();`来删除最后一个文件的所有信息  
eg: `manger->DeleteLastFiles()`  

`vector<QString>& GetBackSuffix()`来获取该文件能够识别的后缀库  
eg: `vector<QString>& qstr = GetBackSuffix();`  

-------  

**Files 结构体内容**  

`QString fileName` - 文件名(无后缀)  
`QString suffix` - 文件后缀(单后缀)  
`QString modifyTime` - 修改时间  
`QString filePath` - 文件路径  
`int size` - 文件大小  