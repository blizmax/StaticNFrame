rem ����ǰ�ļ����е�����Э���ļ�ת��Ϊlua��C++�ļ�  
for %%i in (*.proto) do (    
echo %%i  
protoc.exe --plugin=protoc-gen-lua=".\protoc-gen-lua-master\plugin\protoc-gen-lua.bat" --lua_out=. %%i  
protoc -I=./ --cpp_out=. %%i
)  
pause 
echo end  
rem �򲹶�
repareprotobuf.exe
rem ɾ���м��ļ�

del *.pb.cc
del *.pb.h
copy /y *.lua ..\
del *.lua
pause 