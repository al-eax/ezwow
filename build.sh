#mkdir build
cmake -B./build/ -H. || exit 1
cmake --build ./build/ || exit 1