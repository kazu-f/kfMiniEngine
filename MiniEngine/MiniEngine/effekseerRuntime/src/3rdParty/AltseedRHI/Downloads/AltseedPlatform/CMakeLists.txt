cmake_minimum_required(VERSION 2.8)
project(AltseedPlatform CXX)

if (MSVC)
	#
else()
	set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC" ${CMAKE_CXX_FLAGS})
endif()

add_subdirectory(src)
add_subdirectory(src_test)
