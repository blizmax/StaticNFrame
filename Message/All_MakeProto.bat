set OutDir="../Server/NFMessageDefine/"
protoc.exe  --cpp_out=%OutDir%  ./mysql_db.proto
pause
