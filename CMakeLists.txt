cmake_minimum_required(VERSION 3.12)

# Use hunter to download and build some third party dependencies
include("external/cmake/HunterGate.cmake")
huntergate(URL "https://github.com/cpp-pm/hunter/archive/v0.23.279.tar.gz" SHA1
           "0a8ede485c3e9c1ceed8ccb989ab6c0aba1f4db7")

project(adif)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

set(CMAKE_CXX_STANDARD 17)

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_definitions(-DDEPENDECY_CHECK)

add_library(
  adif STATIC
  di/idiclientprovider.h
  di/idiclientprovider.cpp
  di/idiclientinjector.h
  di/idiclientinjector.cpp
  di/dependencyprovider_impl.cpp
  di/dependencyprovider_impl.h
  di/dependencyprovider.h
  ds/datastore.h)

# add_library(ds STATIC ds/datastore.h ) set_target_properties(do PROPERTIES
# LINKER_LANGUAGE CXX) target_include_directories(do PRIVATE "di")

# example
add_executable(adif_example example/example.cpp)
target_include_directories(adif_example PRIVATE "di")
target_link_libraries(adif_example adif)

# tests di
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows") # disable for now, there is a
                                             # linker problem...
  file(GLOB TEST_DI_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test_di/*.cpp"
       "${CMAKE_CURRENT_SOURCE_DIR}/test_di/*.h")
  add_executable(di_test ${TEST_DI_SRC_FILES})
  target_include_directories(di_test PRIVATE "di")
  target_link_libraries(di_test GTest::gtest)
  target_link_libraries(di_test GTest::gmock)
  target_link_libraries(di_test adif)

  enable_testing()
  add_test(NAME di_test COMMAND di_test)
endif()

# tests do
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows") # disable for now, there is a
                                             # linker problem...
  file(GLOB TEST_DO_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test_ds/*.cpp"
       "${CMAKE_CURRENT_SOURCE_DIR}/test_ds/*.h")
  add_executable(ds_test ${TEST_DO_SRC_FILES})
  target_include_directories(
    ds_test
    PRIVATE "ds"
    PRIVATE "di")
  target_link_libraries(ds_test GTest::gtest)
  target_link_libraries(ds_test GTest::gmock)
  target_link_libraries(ds_test adif)

  enable_testing()
  add_test(NAME ds_test COMMAND ds_test)
endif()
