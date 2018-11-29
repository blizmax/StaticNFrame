@echo off
echo Building dependencies...

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64

cd protobuf/cmake
md build
cd build
cmake -G "Visual Studio 14 2015 Win64" ..
devenv.com libprotobuf.sln /build Release
cd ..
cd ..
cd ..


pause