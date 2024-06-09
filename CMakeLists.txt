cmake_minimum_required(VERSION 2.8)

project(MyKV_Store)

set(HEAD_PATH ${PROJECT_SOURCE_DIR}/include)
set(EXE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(EXE_NAME mykv_store)

# project version
set(MyKV_Store_VERSION_MAJOR 0)
set(MyKV_Store_VERSION_MINOR 0)
set(MyKV_Store_VERSION_PATCH 0)
message(STATUS "[VERSION] the version of the MyKV_Store is ${MyKV_Store_VERSION_MAJOR}.${MyKV_Store_VERSION_MINOR}.${MyKV_Store_VERSION_PATCH}")

add_subdirectory(${PROJECT_SOURCE_DIR}/src)