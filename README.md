# JasonObject

## Main idea

A small wrapper around boost-json to make handling of keys_ easier.
The idea behind the library is make the manipulation of json objects a bit easier, minimise the need for tedious
try-/catch-blocks and iterations. Also front-/back-insertions and retrieval in list was added for convenience.

## How to use the library

A json object can be created from a json string or loaded from file.
```c++
auto jsonObj = JsonObject(R"(
{
    "key1":"value1",
    "key2":[
                1,
                2,
                3
           ]
}
)");
```
or
```c++
auto jsonObj = JsonObject{};
jsonObj.load("/path/to/my/data.json");
```

You can set existing values by specifying a path to the value and the value itself:
```c++
jsonObj.set("key2/[1]", 666);
```
This sets the second value in list of key2 to 666
```json
{
    "key1":"value1",
    "key2":[
                1,
                666,
                3
           ]
}
```
We can also set values with the optional parameter `force` set to true. In this case lists and dictionaries will be
created in object as needed. In the simplest case a dictionary-shaped json can be transformed int a list-shaped json.
```c++
auto jsonObj = JsonObject(R"(
{
    "key": "value"
}
)");
// here jsonObj is a dictionary

jsonObj.set("[0]", 4711, /*force=*/true);
```
Now the json looks like:
```json
[
  4711
]
```
List indices must be enclosed in square brackets and may contain valid non-negative integers or `[^]` (start) or `[$]` (end).
when getting values, the special symbol `[^]` translates to 0, the first index, and `[$]` translates to the last valid
index in the list. So:
```c++
jsonObj.get("key2/[$]");
```
will retrieve the value 1, whilst
```c++
jsonObj.get("key2/[$]");
```
will get the value 3.

**NOTE**: `[^]` and `[$]` can be used at any level where the object has a list container, sop paths like 
          `"key1/[$]/key2/[^]/key3/[$]/[^]"` are possible and the correct index will be selected on each level.

However, when symbols `[^]` and `[$]` are used when setting a value the following applies:

If `set(...)` is used with force set to false, the default:
- `[^]` and `[$]` only select first and last elements along the path
- except at the last key: here `[^]` inserts an element to the start of the list and `[$]` appends an element

If `set(...)` is used with force set to true:
- at the first occurrence of `[^]` or `[$]` an element is inserted/appended
- after that we're on a new branch and all the necessary keys are created on this new branch

For example:
```json
{
  "key": [
    {
      "key2": [1,2,3]
    }
  ]
}
```
When using:
```c++
jsonObj.set("key/[^]/key2/[$]", 666, /*force=*/true);
```
we get the following result:
```json
{
  "key": [
    {
      "key2": [666]
    },
    {
      "key2": [1,2,3]
    }
  ]
}
```

## Underlying container

The class uses `boost_json` as underlying json library, but the necessary abstractions were made via the file
`json_types.h` to make it possible to replace the library with another json implementation by adjusting this file only.

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
