cmake_minimum_required(VERSION 3.15)

# Project name and version
project(AlgoGauge VERSION 1.8)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories for headers
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/CLI_Parsing
    ${CMAKE_SOURCE_DIR}/src/algorithms
    ${CMAKE_SOURCE_DIR}/src/dependencies
)

# Add the source files
set(SOURCES
    src/main.cpp
   
)

# Define the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Set output directories for binaries
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# Disable shared libraries
set(BUILD_SHARED_LIBS OFF)

if(DEFINED CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Toolchain file set to ${CMAKE_TOOLCHAIN_FILE}")
endif()

