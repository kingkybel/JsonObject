# JasonObject

A small wrapper around boost-json to make handling of keys easier.

# Installation

The installation and build is tested on *ubuntu24.04 LTS*

## dependencies

BOOST:

```
# create a directory where you like to clone googletest, eg: ~/Repos and change to it
mkdir ~/Repos ; cd ~/Repos
```

Download, build and install boost, minimum version 1.86.0

```bash
# amend next three lines if you want a newer version of boost
BOOST_MAJOR=1
BOOST_MID=86
BOOST_MINOR=0
BOOST_VER=${BOOST_MAJOR}_${BOOST_MID}_${BOOST_MINOR}
BOOST_VER_DOT=${BOOST_MAJOR}.${BOOST_MID}.${BOOST_MINOR}
wget -O boost_${BOOST_VER}.tar.gz https://sourceforge.net/projects/boost/files/boost/${BOOST_VER_DOT}/boost_${BOOST_VER}.tar.gz/download
tar -xvf boost_${BOOST_VER}.tar.gz
cd boost_${BOOST_VER}
./bootstrap.sh --prefix=/usr/
./b2
sudo ./b2 install
```

googletest:

```bash
# create a directory where you like to clone googletest, eg: ~/Repos and change to it
mkdir ~/Repos ; cd ~/Repos
git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
mkdir build
cmake ..
make -j $(nproc)
sudo make install
```

## use cmake to install the header-only library

```bash
# git clone --recurse-submodules -j $(nproc) https://github.com/kingkybel/DirectedGraph.git
# or:
git clone --recurse-submodules -j $(nproc) git@github.com:kingkybel/JsonObject.git
cd JsonObject
# change the next line to change the install prefix to your liking
INSTALL_PREFIX=/usr
mkdir ./build
cd build
cmake -Wno-dev -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} ..
cmake --build . --parallel $(nproc)
sudo cmake --install .
```

This will install the headers from the include-folder to `${INSTALL_PREFIX}/include/dkyb` and libraries to
`${INSTALL_PREFIX}/lib`

To use the headers in your code, make sure that ${INSTALL_PREFIX} is in the include directories of your project.
Include the file in your code e.g:

```c++
#include <dkyb/json_object.h>
```
Link with library `libdkjsonobject.a`
