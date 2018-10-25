cmake_minimum_required(VERSION 3.1)
project(Signature)

set(CMAKE_CXX_STANDARD 11)
set(executable_name signature)

find_package(Boost COMPONENTS system program_options REQUIRED)
find_package(Threads)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(${executable_name} main.cpp worker.cpp)
  target_link_libraries(${executable_name} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()
