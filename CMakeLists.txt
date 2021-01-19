cmake_minimum_required(VERSION 3.10)

project(adif)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DDEPENDECY_CHECK)

add_library(adif
    STATIC
    di/idiclient.h
    di/idiclient.cpp
    di/dependencyprovider_impl.cpp
    di/dependencyprovider_impl.h
    di/dependencyprovider.h
    )

add_executable(example
    example/example.cpp)

target_include_directories(example PRIVATE "di")
target_link_libraries(example adif)
