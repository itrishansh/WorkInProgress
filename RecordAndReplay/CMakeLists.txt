# comments starts with pound sign
#add_library(testStudent SHARED ${SOURCES})
cmake_minimum_required(VERSION 2.8.9)
project (RecordAndReplay)
include_directories(include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(recordandreplay src/main.c)
