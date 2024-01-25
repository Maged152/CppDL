cmake_minimum_required(VERSION 3.22)
project (reference CXX)

# Source files
file(GLOB_RECURSE SOURCES *.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create the static library
add_library(reference STATIC ${SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/lib/include)

