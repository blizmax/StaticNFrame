| Branch | Build Status |
| - | - | 
| master | [![Build Status](https://travis-ci.org/yigao/StaticNFrame.svg?branch=master)](https://travis-ci.org/yigao/StaticNFrame) |


**Copyright © [Static Game NFrame](https://StaticNFrame.com "Static Game NFrame")**    
Website: https://StaticNFrame.com
Github: https://github.com/yigao/StaticNFrame  
Gitee(码云): https://gitee.com/xiaoyi445/StaticNFrame

## StaticNFrame 
StaticNFrame is a lightweight, agility, scalable, distributed plugin framework written by C++，make it easier and faster to create your own game server, can be used to develop MMO RPG, ARPG, Round play, chess games. Inspired by UnrealEngine and Ogre.

## Features

- Flexible apps, plugins, and modules
- High availability architectures
- General abstract data system
- Interface-oriented and data-oriented programming(IOP & DOP)
- Event-driven and data-driven
- High concurrency and performance of network
- Based on C++ standard, easy to handle and learn
- Cross platform (Include Windows and Linux)
- Less downtime and higher availability
- Lower training and education costs
- Use Excel as configuration files, easier for game designer
- With existed simple Unity3D client for rapid development
- Plentiful plugins(DB, script, http, websocket, etc.)
- Customization service for business customer

### FAQ

Please [create a new issue](https://github.com/yigao/StaticNFrame/issues) to ask a question.


## Architecture

### App Architecture:

![App Architecture](https://github.com/yigao/StaticNFrame/tree/master/Docs/asserts/imgs/AppArchitecture.png)

## Get the Sources:

git clone https://github.com/yigao/StaticNFrame.git

or

svn checkout https://github.com/yigao/StaticNFrame

## Dependencies

- libevent master
- easylogging++ 9.80
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
1. sudo install.sh
2. build_dep.sh
3. build.sh

## Documents

[English](https://github.com/yigao/StaticNFrame/tree/master/Docs/doc_EN.md)

[Chinese](https://github.com/yigao/StaticNFrame/tree/master/Docs/doc_ZH.md)

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

