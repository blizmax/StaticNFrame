set OutDir="../Server/NFMessageDefine/"
  
for %%i in (*.proto) do (    
echo %%i  
protoc.exe --cpp_out=%OutDir% %%i
)
pause
