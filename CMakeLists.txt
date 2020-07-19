cmake_minimum_required(VERSION 3.10)

Project(Tom VERSION 1.0)

set(CMAKE_CXX_STANDARD 17 )
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS -pthread)

add_compile_options(-g)

add_executable(group_msg ./src/group_msg.cpp ./src/Tom_Publisher.cpp ./src/Socket.cpp)