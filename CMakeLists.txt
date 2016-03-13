project(microf)
cmake_minimum_required(VERSION 2.8.11)

# Configuration
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Projects
add_subdirectory(src/3rdparty)
add_subdirectory(src/lib)
add_subdirectory(src/tests)
 
