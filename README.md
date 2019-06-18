| Branch | Build Status | CoverityScan |
| - | - | - |
| master | [![Build Status](https://travis-ci.org/yigao/StaticNFrame.svg?branch=master)](https://travis-ci.org/yigao/StaticNFrame) |      [![CoverityScan](https://scan.coverity.com/projects/15695/badge.svg)](https://scan.coverity.com/projects/yigao-staticnframe) |

**Copyright © [Static Game NFrame](https://StaticNFrame.com "Static Game NFrame")**    
Website: https://StaticNFrame.com
Github: https://github.com/yigao/StaticNFrame  
Gitee(码云): https://gitee.com/xiaoyi445/StaticNFrame

## StaticNFrame 
StaticNFrame是一个由C+编写的轻量级、敏捷性、可扩展的分布式插件框架，分为静态加载和动态加载两种运行方式，可以用来开发各总各样的游戏服务器
功能:
1.可用VS单步调试的分布式服务端，N变1
  一般来说，分布式服务端要启动很多进程，一旦进程多了，单步调试就变得非常困难，导致服务端开发基本上靠打log来查找问题。平常开发游戏逻辑也得开启一大堆进程，不仅启动慢，而且查找问题及其不方便，要在一堆堆日志里面查问题，这感觉非常糟糕，这么多年也没人解决这个问题。StaticNFrame框架使用了类似守望先锋的组件设计，所有服务端内容都拆成了一个个组件，启动时根据服务器类型挂载自己所需要的组件。这有点类似电脑，电脑都模块化的拆成了内存，CPU，主板等等零件，搭配不同的零件就能组装成一台不同的电脑，例如家用台式机需要内存，CPU，主板，显卡，显示器，硬盘。而公司用的服务器却不需要显示器和显卡，网吧的电脑可能不需要硬盘等。正因为这样的设计，StaticNFrame框架可以将所有的服务器组件都挂在一个服务器进程上，那么这个服务器进程就有了所有服务器的功能，一个进程就可以作为整组分布式服务器使用。这也类似电脑，台式机有所有的电脑组件，那它也完全可以当作公司服务器使用，也可以当作网吧电脑。
2.随意可拆分功能的分布式服务端，1变N
  分布式服务端要开发多种类型的服务器进程，比如Login server，gate server，game server，world server, masterserver等等一大堆各种server，传统开发方式需要预先知道当前的功能要放在哪个服务器上，当功能越来越多的时候，比如聊天功能之前在一个中心服务器上，之后需要拆出来单独做成一个服务器，这时会牵扯到大量迁移代码的工作，烦不胜烦。StaticNFrame框架在平常开发的时候根本不太需要关心当前开发的这个功能会放在什么server上，只用一个进程进行开发，功能开发成组件的形式。发布的时候使用一份多进程的配置即可发布成多进程的形式，是不是很方便呢？随便你怎么拆分服务器。只需要修改修改配置文件就可以进行拆分。不同的server加载不同的配置就行了嘛！
3.跨平台的分布式服务端
  支持windows, linux系统， 平时在windows上VS开发， 上线运营是在linux系统跑
4.支持动态加载
  可以运行时动态加载DLL/SO， 来实现更新， 也可以使用LUA开发，来实现动态加载
5.自动生成代码，自动加载配置，自动生成动态加载配置的网站后台系统
  目前还是开发中
## Features

- 插件化，模块化，可以灵活选择不同的插件来组成功能不同的应用程序
- 高可用性体系结构
- 仅仅改动配置或修改程序启动参数就可以实现服务器结构在一个程序里执行还是分N个不同的程序执行
- 跨平台，当前仅仅在windows和centos7,ubuntu上使用过 (Include Windows and Linux)

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
- MSVC >= VS2017  (**<font color=red>Recommend VS2017</font>. Tested in Win7/10**)

## Build and Install

### Windows

##### Prerequisites:

- Windows 10, 8.1, or 7
- Visual Studio 2017 or Visual Studio 2015 Update 3
- Git
- CMake 3.0 +

##### Step:
Using Visual Studio 2017
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

