cd Product
mkdir build
cd build
rm * -rf

cmake -DCMAKE_BUILD_TYPE=Release ../../
make -j4

cd ../..
