echo Building dependencies...

# compiling libevent
cd libevent
chmod a+x ./configure
./configure CPPFLAGS=-fPIC --disable-shared --disable-openssl
make

cp -R -f ./.libs/*.a ../../Lib/Debug/
cp -R -f ./.libs/*.a ../../Lib/Release/
cd ../

# compiling lua 
cd lua
make all
cp -R -f ./*.a ../../Lib/Debug/
cp -R -f ./*.a ../../Lib/Release/
cd ../

# compiling protobuf
cd protobuf
chmod a+x ./configure
./configure CXXFLAGS=-fPIC
make

cp -R -f ./src/.libs/*.a ../../Lib/Debug/
cp -R -f ./src/.libs/*.a ../../Lib/Release/

cd ../

# compiling Theron
cd Theron
make library mode=debug boost=off c++11=on posix=on shared=on
cp -r -f ./Lib/libtherond.a ../../Lib/Debug/
cp -r -f ./Lib/libtherond.a ../../Lib/
make clean
make library mode=release boost=off c++11=on posix=on shared=on
cp -r -f ./Lib/libtheron.a ../../Lib/Release/
cp -r -f ./Lib/libtheron.a ../../Lib/
make clean
cd ../

# TODO: other libs
cd gperftools
chmod a+x ./configure
./configure --enable-frame-pointers
make
cp -R -f ./src/.libs/*.a ../../Lib/Debug/
cp -R -f ./src/.libs/*.a ../../Lib/Release/
cd ../


# back to main dir
pwd
