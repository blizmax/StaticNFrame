cd Tools

tar -xvf cmake-3.3.2.tar.gz
cd cmake-3.3.2
chmod a+x ./configure
./configure
make
sudo make install
cd ..
rm -rf cmake-3.3.2

cd ..
