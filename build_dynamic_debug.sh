cd Product
mkdir build_dynamic_debug
cd build_dynamic_debug
rm * -rf

cmake -DCMAKE_BUILD_TYPE=DynamicDebug ../../
make -j4

cd ../..
