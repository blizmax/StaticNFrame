set OutDir="../Server/NFMessageDefine/"
protoc.exe --cpp_out=%OutDir%  ./mysql_db.proto
protoc.exe --cpp_out=%OutDir%  ./server_msg.proto
pause
