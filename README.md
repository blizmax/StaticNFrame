| Branch | Build Status | CoverityScan |
| - | - | - |
| master | [![Build Status](https://travis-ci.org/yigao/StaticNFrame.svg?branch=master)](https://travis-ci.org/yigao/StaticNFrame) |      [![CoverityScan](https://scan.coverity.com/projects/15695/badge.svg)](https://scan.coverity.com/projects/yigao-staticnframe) |

**Copyright © [Static Game NFrame](https://StaticNFrame.com "Static Game NFrame")**    
Website: https://StaticNFrame.com
Github: https://github.com/yigao/StaticNFrame  
Gitee(码云): https://gitee.com/xiaoyi445/StaticNFrame

## StaticNFrame 
StaticNFrame是一个由C+编写的轻量级、敏捷性、可扩展的分布式插件框架，从NoachNFrame开源服务器改变而来，分为静态加载和动态加载两种运行方式，可以用来开发各总各样的游戏服务器，笔者自己主要用来开发小型游戏，使用LUA来实现逻辑.

## Features

- 插件化，模块化，可以灵活选择不同的插件来组成功能不同的应用程序
- 高可用性体系结构
- 仅仅改动配置或修改程序启动参数就可以实现服务器结构在一个程序里执行还是分N个不同的程序执行
- 跨平台，当前仅仅在windows和centos7上使用过 (Include Windows and Linux)

### FAQ

Please [create a new issue](https://github.com/yigao/StaticNFrame/issues) to ask a question.


## Get the Sources:

git clone https://github.com/yigao/StaticNFrame.git

or

svn checkout https://github.com/yigao/StaticNFrame

## Dependencies

- libevent master
- splog
- google protobuf 3.5.x
- gperftools master

## Supported Compilers

- GCC >= 4.8 (**<font color=red>Recommend</font>. Tested in Ubuntu 15.04**)
- MSVC >= VS2015 update 3 (**<font color=red>Recommend VS2017</font>. Tested in Win7/10**)

## Build and Install

### Windows

##### Prerequisites:

- Windows 10, 8.1, or 7
- Visual Studio 2017 or Visual Studio 2015 Update 3
- Git
- CMake 3.0 +

##### Step:
Using Visual Studio 2015
1. Pull all source
2. Run command line 
```batch
可以编译依赖也可以不编译，依赖需要的库已经上传，如果编译的话，需要按照cmake
cd StaticNFrame/Server/Dependencies
build_dep.bat
```
3. Open the solution: `Server/StaticNFrame.sln`
4. Build the solution`StaticNFrame.sln`
5. Run the binary file by `Product/Debug/Run.bat`

### linux

##### Prerequisites:

- g++ 4.8+ (need C++ 11)
- Git
- CMake 3.0+
- libtool
- automake

Using CMake
1. 首先按照一些依赖，主要是编译需要的g++之类的sudo install.sh
2. 编译依赖文件，也可以不去编译，需要的库文件已经编译好，并上传build_dep.sh
3. 编译基础引擎，服务器框架引擎，具体服务器逻辑实现引擎build.sh

## Examples

Please see `Frame/Examples`.

----------

Our team has always specialized in the agile development and try to help different teams to develop game project quickly and easily. After years of development, we create this frame. We are still improving it.

----------

## Some Awesome projects

[PSS](https://github.com/freeeyes/PSS)
- Author: freeeyes
- Description: 基于插件的跨平台网络服务器框架

----------

#### The relationship with NF

This branch come from [NF](https://github.com/ketoo/NoahGameFrame), we used to be the **CORE DEVELOPERS** of NF. We create **ARK** because some different design philosophies, through our new ideas and modifications, now this project is a fresh project which has many different features.

