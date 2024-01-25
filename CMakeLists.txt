cmake_minimum_required(VERSION 3.22)

project (CPPDL CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(lib)
add_subdirectory(test)

add_executable(run run.cpp)
target_link_libraries(run PUBLIC shakhbat_dl_lib)
target_include_directories(run PUBLIC lib/include)