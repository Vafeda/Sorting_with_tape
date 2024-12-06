cmake_minimum_required(VERSION 3.30)

project(SortTape)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_FILES src/Sort_tape.cpp src/tape.cpp src/sort.cpp)
add_executable(Sort_tape ${SRC_FILES})
target_include_directories(Sort_tape PRIVATE ${CMAKE_SOURCE_DIR}/header)

add_executable(Unit_test tests/Unit_test.cpp)