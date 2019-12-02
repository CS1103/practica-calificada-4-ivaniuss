# CMakeList.txt : CMake project for image_processing, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(pc4)
set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic -Wextra")
endif()

set(OpenCV_DIR "C:\\Users\\Ivan Mamani\\Downloads\\opencv\\build")
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)

add_executable (pc4 main.cpp lodepng.cpp lodepng.h)
