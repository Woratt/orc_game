
if [ -d "build" ]; then
    rmdir build
fi

mkdir build
cd build

cmake ..
make
./game

