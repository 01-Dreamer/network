cmake_minimum_required(VERSION 2.8)
project(Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(server server.cpp)
add_executable(client client.cpp)