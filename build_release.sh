cd Product
mkdir build_release
cd build_release
rm * -rf

cmake -DCMAKE_BUILD_TYPE=Release ../../
make -j4

cd ../..
