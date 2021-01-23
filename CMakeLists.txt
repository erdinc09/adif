cmake_minimum_required(VERSION 3.12)

# Use hunter to download and build some third party dependencies
include("cmake/HunterGate.cmake")
huntergate(URL "https://github.com/cpp-pm/hunter/archive/v0.23.279.tar.gz" SHA1 "0a8ede485c3e9c1ceed8ccb989ab6c0aba1f4db7")


project(adif)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

set(CMAKE_CXX_STANDARD 17)

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)


add_definitions(-DDEPENDECY_CHECK)

add_library(adif
    STATIC
    di/idiclient.h
    di/idiclient.cpp
    di/dependencyprovider_impl.cpp
    di/dependencyprovider_impl.h
    di/dependencyprovider.h
    )

#example
add_executable(adif_example
    example/example.cpp)
target_include_directories(adif_example PRIVATE "di")
target_link_libraries(adif_example adif)

#tests
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows") # disable for now, there is a linker problem...
    file(GLOB TEST_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/test/*.h")
    add_executable(adif_test ${TEST_SRC_FILES})
    target_include_directories(adif_test PRIVATE "di")
    target_link_libraries(adif_test GTest::gtest)
    target_link_libraries(adif_test GTest::gmock)
    target_link_libraries(adif_test adif)

    enable_testing()
    add_test(NAME adif_test COMMAND adif_test)
endif()
