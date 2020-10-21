cmake_minimum_required(VERSION 3.17)
project(MarkovNet)

set(CMAKE_CXX_STANDARD 11)

add_executable(MarkovNet main.cpp NormalDistribution.h NormalDistribution.cpp Node.cpp Node.h ComplexState.cpp ComplexState.h Edge.h Edge.cpp Network.cpp Network.h ExponentialDistribution.cpp ExponentialDistribution.h ModelInstance.cpp ModelInstance.h TestingDistributions.cpp TestingDistributions.h UniformDistribution.cpp UniformDistribution.h)