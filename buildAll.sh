cd Server/Dependencies/
chmod a+x ./build_dep.sh
./buildDep.sh
cd ../../

cd Product
mkdir build
cd build
rm * -rf

if [ $1 == "-R" ]
then
	cmake -DCMAKE_BUILD_TYPE=Release ../../
else 
	cmake -DCMAKE_BUILD_TYPE=Debug ../../
fi

if [ $2 == "-C" ]
then
	make clean
	make -j4
else
	make -j4
fi

cd ../..
