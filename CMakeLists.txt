cmake_minimum_required(VERSION 3.4)
project(File_T_5)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp ClientData.cpp ClientData.h IndexP.cpp IndexP.h IndexS.cpp IndexS.h)
add_executable(File_T_5 ${SOURCE_FILES})