rem ����ǰ�ļ����е�����Э���ļ�ת��Ϊlua��C++�ļ�  

protoc.exe --plugin=protoc-gen-lua=".\protoc-gen-lua-master\plugin\protoc-gen-lua.bat" --lua_out=. st_human.proto  
protoc -I=./ --cpp_out=. st_human.proto 

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