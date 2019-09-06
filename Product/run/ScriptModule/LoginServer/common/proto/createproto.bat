rem 将当前文件夹中的所有协议文件转换为lua和C++文件  
for %%i in (*.proto) do (    
echo %%i  
protoc.exe --plugin=protoc-gen-lua=".\protoc-gen-lua-master\plugin\protoc-gen-lua.bat" --lua_out=. %%i  
protoc -I=./ --cpp_out=. %%i
)  
pause 
echo end  
rem 打补丁
repareprotobuf.exe
rem 删除中间文件

del *.pb.cc
del *.pb.h
copy /y *.lua ..\
del *.lua
pause 