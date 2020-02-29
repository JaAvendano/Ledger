#Set inimum version for CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# needed for cmake to find header files during compie time.
include_directories(src/main/cpp)

#

add_library(Main
        static
        src/main/cpp/Main.cpp)