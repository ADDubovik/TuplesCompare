cmake_minimum_required(VERSION 3.5)

project(Compare LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (SRC
	for_each_in_tuple.h
	main.cpp
)

add_executable(Compare ${SRC})
