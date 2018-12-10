set OutDir="../Server/NFMessageDefine/"
protoc.exe --cpp_out=%OutDir%  ./mysql_db.proto
protoc.exe --cpp_out=%OutDir%  ./db_base.proto
protoc.exe --cpp_out=%OutDir%  ./server_msg.proto
protoc.exe --cpp_out=%OutDir%  ./login_msg.proto
protoc.exe --cpp_out=%OutDir%  ./proxy_msg.proto
pause
