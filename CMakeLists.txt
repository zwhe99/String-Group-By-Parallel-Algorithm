cmake_minimum_required(VERSION 3.16)
project(ParallelProject C)

set(CMAKE_C_STANDARD 99)
# added -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -fopenmp")


add_executable(ParallelProject main.c string_group_by.h string_group_by.c test.h test.c)

message(STATUS "Checking OpenMP")
find_package(OpenMP)
IF(OPENMP_FOUND)
    message("Found OpenMP!")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ELSE()
    message("Missed OpenMP!")
ENDIF()