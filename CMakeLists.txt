cmake_minimum_required(VERSION 3.8)
project(JsonObject CXX)

if(POLICY CMP0167)
        cmake_policy(SET CMP0167 NEW)
endif()

include(CTest)

set(DKYB_CMAKE_COMMON_LOCAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-common" CACHE PATH "Path to local dkyb cmake-common checkout")
set(DKYB_CMAKE_COMMON_GIT_REPOSITORY "https://github.com/kingkybel/cmake-common.git" CACHE STRING "dkyb cmake-common repository")
set(DKYB_CMAKE_COMMON_GIT_TAG "v0.1.0" CACHE STRING "Pinned dkyb cmake-common tag")

set(_dkyb_common_module "${DKYB_CMAKE_COMMON_LOCAL_PATH}/cmake/DkybBuildSettings.cmake")
if(EXISTS "${_dkyb_common_module}")
        include("${_dkyb_common_module}")
else()
        include(FetchContent)
        FetchContent_Declare(
                dkyb_cmake_common
                GIT_REPOSITORY "${DKYB_CMAKE_COMMON_GIT_REPOSITORY}"
                GIT_TAG "${DKYB_CMAKE_COMMON_GIT_TAG}"
        )
        FetchContent_MakeAvailable(dkyb_cmake_common)
        include("${dkyb_cmake_common_SOURCE_DIR}/cmake/DkybBuildSettings.cmake")
endif()

dkyb_apply_common_settings(
        EXTRA_CXX_FLAGS
        -DBOOST_ALLOW_DEPRECATED_HEADERS
        -DBOOST_BIND_GLOBAL_PLACEHOLDERS
)
dkyb_update_submodules()

set(CMAKE_FIND_LIBRARY_PREFIXES lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

set(Boost_USE_STATIC_LIBS ON CACHE BOOL "Prefer static Boost libraries")
find_package(Boost 1.86.0 CONFIG REQUIRED COMPONENTS json)
message(STATUS "Boost_DIR '${Boost_DIR}'")

include_directories(
        BEFORE SYSTEM ${CMAKE_SOURCE_DIR}/include
)
link_directories(
        BEFORE SYSTEM ${CMAKE_SOURCE_DIR}/build/lib
        /usr/local/lib
)

add_subdirectory(src)
if(BUILD_TESTING)
        add_subdirectory(test)
endif()

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=* -p ${CMAKE_SOURCE_DIR}/build")

file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)

install(FILES ${HEADER_FILES}
        DESTINATION include/dkyb)

install(TARGETS dkjsonobject
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)
