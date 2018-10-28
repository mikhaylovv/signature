cmake_minimum_required(VERSION 3.1)
project(Signature)

set(CMAKE_CXX_STANDARD 11)
set(executable_name signature)

find_package(Boost 1.66.0 COMPONENTS 
    system 
    program_options 
    unit_test_framework REQUIRED)

find_package(Threads)

add_subdirectory(src)
add_subdirectory(tests)
