cmake_minimum_required( VERSION 2.8)
project(useEigen)

# Release不包含调试信息，会对程序的大小和运行速度进行优化
set(CMAKE_BUILD_TYPE "Release")


include_directories("/usr/include/eigen3")
add_executable( eigenMatrix eigenMatrix.cpp )
