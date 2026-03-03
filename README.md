![JsonObject banner](./assets/banners/json_object_banner.svg)

# JsonObject

`JsonObject` is a C++23 library built on Boost.JSON for reading and modifying nested JSON values using slash-separated key paths.

## What this repository contains

- Core library:
  - `include/json_object.h`
  - `include/json_key_path.h`
  - `include/json_types.h`
  - `src/json_object.cc`
  - `src/json_key_path.cc`
- Unit tests with GoogleTest in `test/`
- CMake build based on shared settings from `cmake-common/`

## Core capabilities

- Access nested JSON values with string paths like `user/profile/name`
- Access array items with index keys like `[0]`, `[1]`
- Use special array symbols:
  - `[^]` first element (or prepend on `set`)
  - `[$]` last element (or append on `set`)
- Optional default values for safe reads
- Optional `force=true` writes to create compatible intermediate containers
- File I/O helpers:
  - `load(filename)`
  - `write(filename, indent)`

## Path syntax

- Segments are separated by `/`
- Object keys are plain strings, for example: `settings/theme`
- Array indices are bracketed, for example: `users/[0]/name`
- Valid index symbols: `[0]`, `[1]`, ..., `[^]`, `[$]`
- Invalid string keys include empty strings, whitespace-only strings, numeric-only strings, or keys containing `[`, `]`, `\n`, `\r`

## Examples

### 1) Basic get/set

```cpp
#include <dkyb/json_object.h>

using util::JsonObject;

JsonObject obj(R"({"user":{"name":"Ada","age":36}})");

obj.set("user/name", "Ada Lovelace");
auto name = obj.get("user/name");
```

### 2) Defaults and compatibility checks

```cpp
#include <dkyb/json_object.h>

using util::JsonObject;

JsonObject obj(R"({"user":{"name":"Ada"}})");

// Missing key in a compatible object path -> returns default
auto city = obj.get("user/city", "unknown");

// Incompatible path still throws (object vs array mismatch)
// obj.get("user/[0]", "fallback");
```

### 3) Array operations with special symbols

```cpp
#include <dkyb/json_object.h>

using util::JsonObject;

JsonObject arr(R"([1,2,3])");

arr.set("[^]", 0);  // prepend -> [0,1,2,3]
arr.set("[$]", 4);  // append  -> [0,1,2,3,4]

auto first = arr.get("[^]");
auto last  = arr.get("[$]");
```

### 4) Force-create intermediate structures

```cpp
#include <dkyb/json_object.h>

using util::JsonObject;

JsonObject obj("{}");

// Without force this would throw due to missing path/container
obj.set("a/[0]/name", "node-0", true);
// Result is compatible structure created on demand
```

### 5) Using JsonKeyPath directly

```cpp
#include <dkyb/json_key_path.h>
#include <dkyb/json_object.h>

util::JsonObject obj(R"({"items":[{"id":7}]})");
util::JsonKeyPath path("items/[0]/id");

auto id = obj.get(path);
```

### 6) Load and write files

```cpp
#include <dkyb/json_object.h>

util::JsonObject obj;
obj.load("input.json");
obj.set("meta/version", 2, true);
obj.write("output.json", 2);  // pretty-print with 2-space indentation
```

## Build and test

### Dependencies

- CMake
- C++23 compiler
- Boost (Boost.JSON component)
- GoogleTest (for tests)

### Configure and build

```bash
git clone --recurse-submodules -j "$(nproc)" git@github.com:kingkybel/JsonObject.git
cd JsonObject
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel "$(nproc)"
```

### Run tests

```bash
ctest --test-dir build --output-on-failure
# or: ./build/Debug/bin/run_tests
```

## Install

```bash
INSTALL_PREFIX=/usr
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}"
cmake --build build --parallel "$(nproc)"
sudo cmake --install build
```

Headers are installed to `${INSTALL_PREFIX}/include/dkyb` and the library to `${INSTALL_PREFIX}/lib`.

## Powered by

Reduce the smells, keep on top of code-quality. Sonar Qube is run on every push to the `main` branch on GitHub.

[![SonarQubeCloud](assets/icons/logo-sonarqube-cloud-small.png)](https://sonarcloud.io/project/overview?id=kingkybel)
