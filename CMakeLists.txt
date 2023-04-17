cmake_minimum_required(VERSION 3.25)
project(twingate_memory_management)

set(CMAKE_CXX_STANDARD 14)

add_executable(twingate_memory_management main.cpp MemoryManager.cpp BlockDescriptor.h)
