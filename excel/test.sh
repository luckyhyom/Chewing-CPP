#build
rm -rf ./build
cmake -S . -B build
cd build

# linux
# make -j$(nproc)
# macOS
make -j$(sysctl -n hw.ncpu)

ctest -V