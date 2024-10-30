cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AlgoGauge)
add_executable( AlgoGauge 
        main.cpp
        algorithms/sort_7algs.cpp
        # algorithms/hash_algs.cpp
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
