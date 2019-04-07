set OutDir="../Server/NFMessageDefine/"
protoc.exe --cpp_out=%OutDir%  ./mysql_db.proto
protoc.exe --cpp_out=%OutDir%  ./db_base.proto
protoc.exe --cpp_out=%OutDir%  ./server_msg.proto
protoc.exe --cpp_out=%OutDir%  ./sqlite_db.proto

protoc.exe --cpp_out=%OutDir%  ./st_human.proto
protoc.exe --cpp_out=%OutDir%  ./st_human_packet_code.proto
pause
