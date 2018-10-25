cmake_minimum_required(VERSION 3.1)
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(${executable_name} ${SOURCE})

target_link_libraries(${executable_name} 
    ${Boost_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT}
)

