LIBNAP
=============

A Cross-Platform **LIB**rary of **N**etwork **AP**plication tools

Version 0.0.1



## 1.BUILDING AND INSTALLATION 构建和安装

### Linux  (Make)
------
```bash
$ git clone https://github.com/staticn/libnap.git
$ cd ./libnap
$ make
$ make install
```

**If the compilation is successful, Then the output folder under the project directory will have static link library files.**

**如果编译成功，项目目录下output文件夹里会有静态链接库文件**







### Windows  (Visual Studio)
------
```powershell
$ git clone https://github.com/staticn/libnap.git
$ cd ./libnap/win
$ ./libnap.sln
```

**If the compilation is successful, Then the install folder under the project directory will have static link library files  and header files.**

**如果编译成功，则项目目录下的install文件夹将具有静态链接库文件和头文件。**



## 2.Other content



####  In Linux, you can use the following make command to compile according to different needs

####  在Linux中，可以使用以下make命令根据不同需求进行编译

------
```bash
make / make all : 编译libnap 编译测试文件 并运行测试
make build : 编译libnap 编译测试文件
make test : 编译测试文件
make libnap : 编译libnap
make clean : 清除编译的文件
make fast : 启用多核快速安静编译
```



####  Platform requirements 平台要求
------

##### Windows
```
Visual Studio 2017 / 2019
Windows SDK Version > 10.0
```

##### Linux
```
G++ Version >= 4.8.5
Make Version >= 3.8
```



