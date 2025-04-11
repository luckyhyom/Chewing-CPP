## install dependencies

### macOS
```bash
brew install cmake
brew install googletest
```

### Ubuntu
CMake
```bash
wget https://github.com/Kitware/CMake/releases/download/v3.31.3/cmake-3.31.3.tar.gz
gunzip cmake-3.31.3.tar.gz 
tar -xvf cmake-3.31.3.tar 
cd cmake-3.31.3/
./bootstrap && make && sudo make install
```
[googletest](https://github.com/google/googletest/blob/main/googletest/README.md)
```bash
git clone https://github.com/google/googletest.git -b v1.15.2
cd googletest        # Main directory of the cloned repository.
mkdir build          # Create a directory to hold the build output.
cd build
cmake ..             # Generate native build scripts for GoogleTest.
```

## excute shell script
```bash
./test.sh
```
