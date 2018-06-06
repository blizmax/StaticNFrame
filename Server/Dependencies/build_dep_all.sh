echo Building dependencies...

#compiling mysql
cd mysql
cmake .
make clean
make

cp -R -f ./libmysql/libmysqlclient.a ../../../Product/Lib/Debug/
cp -R -f ./libmysql/libmysqlclient.a ../../../Product/Lib/Release/
cp -R -f ./libmysql/libmysqlclient.so ../../../Product/Lib/Debug/
cp -R -f ./libmysql/libmysqlclient.so ../../../Product/Lib/Release/
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

# compiling lua 
cd lua
make clean
make all
cp -R -f ./*.a ../../../Product/Lib/Debug/
cp -R -f ./*.a ../../../Product/Lib/Release/
cd ../

# compiling protobuf
cd protobuf
chmod a+x ./configure
#./configure --disable-shared CFLAGS="-fPIC" CXXFLAGS="-fPIC"
./configure CFLAGS="-fPIC" CXXFLAGS="-fPIC"
make clean
make

cp -R -f ./src/.libs/*.a ../../../Product/Lib/Debug/
cp -R -f ./src/.libs/*.a ../../../Product/Lib/Release/


cp -R -f ./src/.libs/libprotobuf.so* ../../../Product/Lib/Debug/
cp -R -f ./src/.libs/libprotobuf.so* ../../../Product/Lib/Release/

cd ../

# compiling Theron
cd Theron
make clean
make library mode=debug boost=off c++11=on posix=on shared=on
cp -r -f ./Lib/libtheron.a ../../../Product/Lib/Debug/
make clean
make library mode=release boost=off c++11=on posix=on shared=on
cp -r -f ./Lib/libtheron.a ../../../Product/Lib/Release/
make clean
cd ../

# TODO: other libs
cd gperftools
chmod a+x ./configure
./configure
make clean
make
cp -R -f ./.libs/libtcmalloc.a ../../../Product/Lib/Debug/
cp -R -f ./.libs/libtcmalloc.a ../../../Product/Lib/Release/
cp -R -f ./.libs/libprofiler.a ../../../Product/Lib/Debug/
cp -R -f ./.libs/libprofiler.a ../../../Product/Lib/Release/
cp -R -f ./.libs/libtcmalloc_minimal.a ../../../Product/Lib/Debug/
cp -R -f ./.libs/libtcmalloc_minimal.a ../../../Product/Lib/Release/
cd ../


# back to main dir
pwd
