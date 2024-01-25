cmake_minimum_required(VERSION 3.22)
project (lib CXX)

# Source files
file(GLOB_RECURSE  SOURCES source/*.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create the static library
add_library(shakhbat_dl_lib STATIC ${SOURCES})
target_include_directories(shakhbat_dl_lib PUBLIC include)

install(TARGETS shakhbat_dl_lib DESTINATION ${CMAKE_SOURCE_DIR}/package)
install(DIRECTORY include DESTINATION ${CMAKE_SOURCE_DIR}/package)
