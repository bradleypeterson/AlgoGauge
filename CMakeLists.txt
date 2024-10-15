cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 23)

project(AlgoGauge)
add_executable( AlgoGauge main.cpp
        dependencies/cxxopts.hpp
        algorithms/sort_7algs.cpp
        CLI_Parsing/CLI_Parser.hpp
        CLI_Parsing/sorting_algorithm_caller.hpp
        algorithms/RandomNum.hpp
       
)
set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies")

include_directories(dependencies)
include_directories(MultiLangauge**)
include_directories(testing)




# Specify the executable (replace main.cpp with your actual C++ source file)

# Specify the directory containing the dependencies, including the .mjs file

# Copy the Algogauge.mjs file to the build directory
# add_custom_command(
#     TARGET AlgoGauge POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy
#     ${DEPENDENCIES_DIR}/Algogauge.mjs
#     $<TARGET_FILE_DIR:AlgoGauge>
# )

# Make sure CMake knows about the dependencies directory for .cpp files
# include_directories(${DEPENDENCIES_DIR})
