cmake_minimum_required(VERSION 3.5)

project(GA_Project)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")

find_package(OpenCV REQUIRED)
find_package(C2GA REQUIRED)
find_package(Eigen3 REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -std=c++17 -Wfatal-errors")

file(GLOB_RECURSE source_files src/*.cpp src/*.hpp)

add_executable(GA_Project ${source_files})

include_directories(${OpenCV_INCLUDE_DIR})
include_directories(${C2GA_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})

target_link_libraries(GA_Project ${OpenCV_LIBS})
target_link_libraries(GA_Project ${C2GA_LIBRARIES})


