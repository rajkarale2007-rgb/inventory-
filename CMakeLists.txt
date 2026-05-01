cmake_minimum_required(VERSION 3.10)
project(InventoryApp C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directory
include_directories(include)

# Source files
set(SOURCES
    src/inventory.c
    src/InventoryManager.cpp
    src/main.cpp
)

# Executable
add_executable(inventory_app ${SOURCES})
