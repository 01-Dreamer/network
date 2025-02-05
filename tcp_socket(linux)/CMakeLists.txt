cmake_minimum_required(VERSION 3.16)
project(project)

set(CMAKE_CXX_STANDARD 11)

add_executable(server server.cpp)
add_executable(client client.cpp)
