call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64

cd mongo
mkdir cmake_build
cd cmake_build
cmake -G "Visual Studio 14 2015 Win64"  ..
msbuild.exe /p:Configuration=Release ALL_BUILD.vcxproj

cd ../..

pause