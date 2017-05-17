cmake_minimum_required(VERSION 3.6)
project(holdem_core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp src/cardlogic.cpp src/holdemcardlogic.cpp src/holdemlogic.cpp src/cmd.cpp)
add_executable(holdem_core ${SOURCE_FILES})