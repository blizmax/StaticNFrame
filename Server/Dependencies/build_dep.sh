echo Building dependencies...

#compiling mysql
cd mysql
cmake .
make clean
make

cp -R -f ./libmysql/libmysqlclient.a ../../../Product/Lib/Debug/
cp -R -f ./libmysql/libmysqlclient.a ../../../Product/Lib/Release/
cp -R -f ./libmysql/libmysqlclient.so* ../../../Product/Lib/Debug/
cp -R -f ./libmysql/libmysqlclient.so* ../../../Product/Lib/Release/
cd ..


# compiling libevent
cd libevent
chmod a+x ./configure
./configure CPPFLAGS=-fPIC --disable-shared --disable-openssl
make clean
make

cp -R -f ./.libs/*.a ../../../Product/Lib/Debug/
cp -R -f ./.libs/*.a ../../../Product/Lib/Release/
cd ../

# compiling protobuf
cd protobuf
cd cmake
rm build -rf
mkdir build
cd build
cmake ..
make -j4

cd ../../

cp -R -f ./cmake/build/*.a ../../../Product/Lib/Debug/
cp -R -f ./cmake/build/*.a ../../../Product/Lib/Release/

cd ..

# back to main dir
pwd
