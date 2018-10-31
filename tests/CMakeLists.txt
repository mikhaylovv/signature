cmake_minimum_required(VERSION 3.1)
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
    )

set(test_name signature_test)

file(GLOB SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_SOURCE_DIR}/src/worker.h
    ${CMAKE_SOURCE_DIR}/src/worker.cpp
)

add_executable(${test_name} ${SOURCE})

target_link_libraries(${test_name} 
    ${Boost_LIBRARIES} 
)

add_test(${test_name} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${test_name})
