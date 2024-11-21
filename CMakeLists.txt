cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AlgoGauge)
add_executable( AlgoGauge 
        main.cpp
        algorithms/sort_7algs.cpp
        algorithms/hash_algs.cpp

)

set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies")

include_directories(dependencies)
include_directories(MultiLangauge**)
include_directories(testing)

