LIBNAP
=============

A Simple Network App Library

Version 1.0



## 1.BUILDING AND INSTALLATION



### Linux  (Make)
------
```bash
$ git clone https://github.com/staticn/libnap.git
$ cd ./libnap
$ make
$ make install
```

#####  If the compilation is successful, Then the output folder under the project directory will have static link library files.





### Windows  (Visual Studio)
------
```powershell
$ git clone https://github.com/staticn/libnap.git
$ cd ./libnap/win
$ ./libnap.sln
```

#####  If the compilation is successful, Then the install folder under the project directory will have static link library files  and header files.





## 2.Other content



####  In Linux, you can use the following make command to compile according to different needs

------
```bash
make / make all : 编译libnap 编译测试文件 并运行测试
make build : 编译libnap 编译测试文件
make test : 编译测试文件
make libnap : 编译libnap
make clean : 清除编译的文件
make fast : 启用多核快速安静编译
```



####  Platform requirements
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



