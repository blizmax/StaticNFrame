cd mongo
mkdir cmake_build
cd cmake_build
cmake ..
make

cp -rf src/libmongoc/libmongoc-static-1.0.a ../../../../Product/Lib/Debug/ 
cp -rf src/libmongoc/libmongoc-static-1.0.a ../../../../Product/Lib/Release/ 
cp -rf src/libbson/libbson-static-1.0.a ../../../../Product/Lib/Debug/
cp -rf src/libbson/libbson-static-1.0.a ../../../../Product/Lib/Release/

