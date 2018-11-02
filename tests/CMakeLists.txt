cmake_minimum_required(VERSION 3.1)
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
    )

set(test_name signature_tests)

file(GLOB SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(${test_name} ${SOURCE})

set_target_properties(${test_name} PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${test_name} 
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

add_test(NAME ${test_name} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${test_name})
