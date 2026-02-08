cmake_minimum_required(VERSION 3.10)
project(EPC_Optimizer VERSION 1.0)

# Specify C++ Standard (SystemC compatible)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add 'include' to the compiler's search path
include_directories(include)

# Define source files
set(SOURCES
    src/main.cpp
    src/EPC_Population.cpp
)

# Create the executable
add_executable(epc_run ${SOURCES})

# Enable standard warnings (Best Practice)
if(MSVC)
    target_compile_options(epc_run PRIVATE /W4)
else()
    target_compile_options(epc_run PRIVATE -Wall -Wextra)
endif()