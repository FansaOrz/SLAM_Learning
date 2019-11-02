cmake_minimum_required( VERSION 2.8)

project( Test_Code )

add_library( hello_shared SHARED libHelloSLAM.cpp)

add_executable( test_code test_code.cpp )
add_executable( use_libHelloSLAM use_libHelloSLAM.cpp)
target_link_libraries( use_libHelloSLAM hello_shared)