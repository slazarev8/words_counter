cmake_minimum_required(VERSION 3.22)
project(ChunkCounter)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE RELEASE)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(ChunkCounter main.cpp Timer.cpp Timer.h CountDispatcher.cpp BufferReader.cpp WordsCounter.cpp BufferTaskSolver.cpp)
