cmake_minimum_required (VERSION 2.6)
project (Grep)
set (CMAKE_CXX_STANDARD 11)
add_executable(mygrep grep.cpp path.cpp KMP.cpp)
