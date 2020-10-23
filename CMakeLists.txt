cmake_minimum_required(VERSION 3.17)
project(MarkovNet)

set(CMAKE_CXX_STANDARD 11)

add_executable(MarkovNet main.cpp Node.cpp Node.h ComplexState.cpp ComplexState.h Edge.h Edge.cpp Network.cpp Network.h Distribution.cpp Distribution.h ModelInstance.cpp ModelInstance.h TestingDistributions.cpp TestingDistributions.h)