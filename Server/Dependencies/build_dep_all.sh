echo Building dependencies...

#compling openssl
cd openssl
chmod a+x ./config
./config

cp -rf libcrypto.a ../../../Product/Lib/Debug/
cp -rf libcrypto.a ../../../Product/Lib/Release/
cp -rf libssl.a ../../../Product/Lib/Debug/
cp -rf libssl.a ../../../Product/Lib/Release/
cp -rf libcrypto.so.1.1 ../../../Product/run/Dynamic_Debug/
cp -rf libssl.so.1.1 ../../../Product/run/Dynamic_Debug/
cp -rf libcrypto.so.1.1 ../../../Product/run/Dynamic_Release/
cp -rf libssl.so.1.1 ../../../Product/run/Dynamic_Release/
cd ..

cd curl
./buildconf
./configure --prefix=/usr/local/curl --with-ssl=/usr/local/openssl --disable-shared

#compiling mysql
cd mysql
cmake .
make

cp -R -f ./libmysql/libmysqlclient.a ../../../Product/Lib/Debug/
cp -R -f ./libmysql/libmysqlclient.a ../../../Product/Lib/Release/
cp -R -f ./libmysql/libmysqlclient.so.18.4. ../../../Product/run/Dynamic_Debug/
cp -R -f ./libmysql/libmysqlclient.so.18.4. ../../../Product/run/Dynamic_Release/

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
cp -R -f ./build/lib/libevent_core.so.2.1.8 ./build/lib/libevent.so.2.1.8 ./build/lib/libevent_extra.so.2.1.8 ../../../Product/run/Dynamic_Debug/
cp -R -f ./build/lib/libevent_core.so.2.1.8 ./build/lib/libevent.so.2.1.8 ./build/lib/libevent_extra.so.2.1.8 ../../../Product/run/Dynamic_Release/
#cp -R -f ./build/include ./linux/
cd ../

# compiling protobuf
cd protobuf
cd cmake
mkdir build
cd build
#cmake .. -DBUILD_SHARED_LIBS=On
cmake ..
make

cd ../../

cp -R -f ./cmake/build/*.a ../../../Product/Lib/Debug/
cp -R -f ./cmake/build/*.a ../../../Product/Lib/Release/
cp -R -f ./cmake/build/libprotobuf.so.3.6.1 ../../../Product/run/Dynamic_Debug/
cp -R -f ./cmake/build/libprotobuf.so.3.6.1 ../../../Product/run/Dynamic_Release/

cd ../

# compiling Theron
cd Theron
make library mode=debug boost=off c++11=on posix=on shared=on
cp -r -f ./Lib/libtheron.a ../../../Product/Lib/Debug/
cp -r -f ./Lib/libtheron.a ../../../Product/Lib/Dynamic_Debug/
make clean
make library mode=release boost=off c++11=on posix=on shared=on
cp -r -f ./Lib/libtheron.a ../../../Product/Lib/Release/
cp -r -f ./Lib/libtheron.a ../../../Product/Lib/Dynamic_Release/
make clean
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
cp -rf src/libmongoc/libmongoc-1.0.so.0.0.0 ../../../../Product/run/Dynamic_Debug/
cp -rf src/libbson/libbson-1.0.so.0.0.0 ../../../../Product/run/Dynamic_Debug/
cp -rf src/libmongoc/libmongoc-1.0.so.0.0.0 ../../../../Product/run/Dynamic_Release/
cp -rf src/libbson/libbson-1.0.so.0.0.0 ../../../../Product/run/Dynamic_Release/
cd ../../


# back to main dir
pwd
