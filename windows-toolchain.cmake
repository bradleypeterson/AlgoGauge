# Toolchain file for cross-compiling to Windows on macOS
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10)

# Specify the compiler
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)


# Set the static linking flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

# Optionally, set library paths for static libraries
set(CMAKE_FIND_LIBRARY_PREFIXES "")
# SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
# SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)


# Specify the path for the linker and libraries
# set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Define the target environment
# set(CMAKE_FIND_ROOT_PATH /opt/homebrew/Cellar/mingw-w64)

# # Search for programs in the build host directories
# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# # Search for headers and libraries in the target directories only
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

