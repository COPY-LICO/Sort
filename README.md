# 接口使用

**ManagerMent方法**

`static ManagerMent* GetInstance()`来获取对应的ManagerMent的指针对象，方便调用ManagerMent的相关操作  
eg: `ManagerMent* manger = ManagerMent::GetInstance();`

`void SaveFilePath(string)`来存入获取到的单个文件路径  
eg: `manger->SaveFilePath(url);`  

`string GetFilesPath()`来获取存入的单个文件路径  
eg: `string url = manger->GetFilesPath();`  
