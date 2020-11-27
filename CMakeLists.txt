cmake_minimum_required(VERSION 3.17)
project(MarkovNet)

set(CMAKE_CXX_STANDARD 17)

add_executable(MarkovNet main.cpp Distribution.cpp Distribution.h ModelInstance.cpp ModelInstance.h TestingDistributions.cpp TestingDistributions.h NetworkArray.cpp NetworkArray.h IO.cpp IO.h ParameterObj.cpp ParameterObj.h Substrate.cpp Substrate.h)