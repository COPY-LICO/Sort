# Git 协作简易指南

> **本项目使用 `develop` 作为主开发分支，`main` 为发布分支**

---

# 克隆仓库
git clone https://github.com/COPY-LICO/sort.git

# 分支功能
> **Github的`develop`分支为主开发分支，`UI`,`backFunction`,`Service`分支为个人开发分支，由对应开发者负责**  
> **每名开发者提交代码时必须将代码提交到``负责分支``上，不能直接提交到`develop`分支尤其`main`上**  

确保使用`git checkout XX`切换到自己的分支然后使用`git push origin XX:YY`来发布到对应的分支上  
XX为本地用于开发的分支，YY为github上负责分支

# 本地协作
> **建议本地使用一个`develop`分支来接受github上来自`develop`分支的同步，然后另起一个自己的分支用于开发**  
创建分支指令`git checkout -b XXX`，如果本地分支名称和github上负责名称一致可以推送时可以简写为`git push origin XXX`  
> **本地请绝对不要修改`.gitignore`文件的内容，自行起名和存放文件时候也需要注意自己的名称是否处于`.gitignore`的排除列表之外**  

> **本地提交代码时可以直接`git add .`来添加所有文件并且提交，不需要提交的文件一件处于`.gitignore`文件中**

# VS相关设置
> **代码请写在3个功能区的文件夹内，文件夹内可以自行创建代码文件，`main.cpp`文件仅作为调用接口，请不要写在里面**  
> **如果遇到情况需要另行创建功能区的文件夹，请和我联系然后更改.sln文件保证功能区文件夹能够正常被VS识别**
功能区文件夹内可以自行创建子类文件夹  
> **VS内编译运行时资源文件加载路径默认为`main.cpp`的路径，与单独运行程序时`xx.exe`的路径不同，编写相对路径代码时候需要注意**

# 分支合并
> **当分支提交到个人开发分支后，需要确保程序无误不存在bug后，再次整合到`develop`分支，随后其余开发者同步`develop`分支的内容**  

# 编码规范
> **程序编码一律使用驼峰法，如SortApplication为大驼峰，sortApplication为小驼峰。函数名称，类名称使用大驼峰，变量名使用小驼峰**  
> **类内私有变量尽量采用_开头，如 _sortApplication**  

> ***绝对不要使用没有意义的变量名和函数名，如a,b,c,d***

创建类对象时候保持`.h .cpp`文件形式。.h作为头文件只用于声明对象，.cpp作为源文件用于定义函数

# 可能用到的命令
`git add .` 将当前目录下的所有文件添加到提交列表里面  
`git commit -m "xxx"` 提交提交列表里面的文件，并且备注为xxx  
`git push origin xx:yy` 将本地的xx分支里面的东西推送到yy分支上  
`git push origin yy` 将当前分支的东西推送到yy分支上  
`git pull origin yy` 将github上yy分支的内容拉取到当前分支上  
`git checkout -b xx` 本地创建一个xx分支  
`git checkout xx` 切换到xx分支  
`git merge origin/xx` 将远程的xx分支合并到本地的当前分支