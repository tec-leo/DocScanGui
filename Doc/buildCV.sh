#!/bin/bash

echo build opencv 5.x on same directory level as DocScanGui

cd ..

echo get opencv 5.x from git

# get from git
git clone https://github.com/opencv/opencv.git
cd opencv
git checkout 5.x

read -p 'Press Enter to compile debug version with shared libs...'

# echo compile debug version with shared libs
# compile debug version with shared libs
cmake -B debug -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Debug
cmake -S . -B debug
cd debug/
cmake --build . -j8

cd ..

read -p 'Press Enter to compile release version with shared libs...'

#echo compile release version with shared libs
# compile release version with shared libs
cmake -B release -DCMAKE_INSTALL_PREFIX=./install -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release
cmake -S . -B release
cd release/
cmake --build . -j8

read -p 'Press Enter to install release in own version...'

#echo install release in own version
# install release in own version
cmake --install .

cd ..

read -p 'Press Enter to copy includes from install to include...'

# copy includes from install to include
cp -r ./install/include/opencv5 ./include

read -p 'opencv is ready ...'

