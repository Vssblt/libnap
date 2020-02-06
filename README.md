LIBNAP
=============

A Simple Network App Library

Version 1.0





## 1.BUILDING AND INSTALLATION



### Linux  (Make)
------
```makefile
$ git clone https://github.com/staticn/libnap.git
$ cd ./libnap
$ make
$ make install
```



### Windows  (Visual Studio)
------
```powershell
$ git clone https://github.com/staticn/libnap.git
$ cd ./libnap/win
$ ./libw.sln
```



## 2.Other content




### In Linux, you can use the following make command to compile according to different needs
------
```makefile
make / make all : 编译libnap 编译测试文件 并运行测试
make build : 编译libnap 编译测试文件
make test : 编译测试文件
make libnap : 编译libnap
make clean : 清除编译的文件
make fast : 启用多核快速安静编译
```



### Platform requirements
------

#### Windows
```
Visual Studio 2017 / 2019
Windows SDK Version > 10.0
```

#### Linux
```
G++ Version >= 4.8.5
Make Version >= 3.8
```





跨平台模块
网络
内存池(gc)
线程池
反射
日志
反射
Mysql API
OpenSSL
