cmake_minimum_required(VERSION 2.8)

aux_source_directory(./ SRC)

include_directories(${HEAD_PATH})

set(EXECUTABLE_OUTPUT_PATH ${EXE_OUTPUT_PATH})

add_executable(${EXE_NAME} ${SRC})