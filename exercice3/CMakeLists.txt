# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name and languages
project(MyProject LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Path to include directories
include_directories(src/include)

# Path for output binaries
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Source files for the executable
set(SOURCES
    src/testMain.cpp
)

# Add the executable
add_executable(testMain ${SOURCES})

# Enable verbose makefile output (optional)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Find and link TBB
find_package(TBB REQUIRED)
if(TBB_FOUND)
    target_link_libraries(testMain PRIVATE TBB::tbb)
endif()

# Optional: Show status messages
message(STATUS "CMake version: ${CMAKE_VERSION}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Output path: ${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "TBB found: ${TBB_FOUND}")
