echo Building dependencies...

# compiling libevent
cd libevent
chmod a+x ./configure
./configure CPPFLAGS=-fPIC --disable-shared --disable-openssl
make clean
make

cp -R -f ./.libs/*.a ../../../Product/Lib/Debug/
cp -R -f ./.libs/*.a ../../../Product/Lib/Release/
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
# back to main dir
pwd
