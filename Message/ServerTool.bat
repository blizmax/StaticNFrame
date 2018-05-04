set /p name=输入文件名，然后回车
protoc.exe  --cpp_out=../Program/Server/Server_rc2/proto  ./%name%.proto
pause