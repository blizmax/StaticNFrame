@echo off
echo Building dependencies...

REM extract libevent libs
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
cd libevent
nmake /f Makefile.nmake
copy libevent.lib ..\\..\\Lib\\Debug\\ /Y
copy libevent.lib ..\\..\\Lib\\Release\\ /Y
copy libevent_core.lib ..\\..\\Lib\\Debug\\ /Y
copy libevent_core.lib ..\\..\\Lib\\Release\\ /Y
copy libevent_extras.lib ..\\..\\Lib\\Debug\\ /Y
copy libevent_extras.lib ..\\..\\Lib\\Release\\ /Y
cd ..

pause