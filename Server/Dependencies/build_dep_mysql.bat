@echo off
echo Building dependencies...

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64

cd mysql
cmake -G "Visual Studio 14 2015 Win64" 
devenv.com libmysql.sln /build Release
cd ..


pause