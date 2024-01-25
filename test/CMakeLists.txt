cmake_minimum_required(VERSION 3.12)
project(test)

enable_testing()

add_subdirectory(googletest)
add_subdirectory(reference)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

#find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/lib/include include googletest/include)

# Find all test source files
file(GLOB_RECURSE TEST_SOURCES *.cpp)

# Create a target for each test file
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} shakhbat_dl_lib reference gtest_main pthread) # Link with Google Test and pthread
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME}) # Register the test with CTest
    add_custom_target(Unit_${TEST_NAME}
    COMMAND  ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}
    )
endforeach()
