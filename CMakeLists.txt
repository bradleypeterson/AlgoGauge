cmake_minimum_required(VERSION 3.22.1)
project(AlgoGauge)

set(CMAKE_CXX_STANDARD 17)

add_executable(AlgoGauge main.cpp
        dependencies/cxxopts.hpp
        algorithms/sort_7algs.cpp
        CLI_Parsing/CLI_Parser.hpp
        algorithms/RandomNum.hpp
        dependencies/Perf.hpp
)
