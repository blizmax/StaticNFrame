echo Building dependencies...

#compling openssl
cd openssl
chmod a+x ./config
./config
make
cp -rf libcrypto.a ../../../Product/Lib/Debug/
cp -rf libssl.a ../../../Product/Lib/Release/
cd ..

#compiling mysql
cd mysql
cmake .
make

cp -R -f ./libmysql/libmysqlclient.a ../../../Product/Lib/Debug/
cp -R -f ./libmysql/libmysqlclient.a ../../../Product/Lib/Release/
cd ..

# compiling libevent
cd libevent
mkdir build
cd build
cmake ..
make
cd ..

cp -R -f ./build/lib/*.a ../../../Product/Lib/Debug/
cp -R -f ./build/lib/*.a ../../../Product/Lib/Release/
#cp -R -f ./build/include ./linux/
cd ../

# compiling protobuf
cd protobuf
cd cmake
mkdir build
cd build
cmake ..
make

cd ../../

cp -R -f ./cmake/build/*.a ../../../Product/Lib/Debug/
cp -R -f ./cmake/build/*.a ../../../Product/Lib/Release/

cd ../

# compiling Theron
cd Theron
make library mode=debug boost=off c++11=on posix=on shared=on
cp -r -f ./Lib/libtheron.a ../../../Product/Lib/Debug/
make library mode=release boost=off c++11=on posix=on shared=on
cp -r -f ./Lib/libtheron.a ../../../Product/Lib/Release/
cd ../


# comiling mongo-c-driver

cd mongo
mkdir cmake_build
cd cmake_build
cmake ..
make

cp -rf src/libmongoc/libmongoc-static-1.0.a ../../../../Product/Lib/Debug/
cp -rf src/libmongoc/libmongoc-static-1.0.a ../../../../Product/Lib/Release/
cp -rf src/libbson/libbson-static-1.0.a ../../../../Product/Lib/Debug/
cp -rf src/libbson/libbson-static-1.0.a ../../../../Product/Lib/Release/
cd ../../


# back to main dir
pwd
