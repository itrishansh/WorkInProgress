# comments starts with pound sign
#add_library(testStudent SHARED ${SOURCES})
cmake_minimum_required(VERSION 2.8.9)
project (hook_exec)
include_directories(include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(testexecl test/execl.c)
add_executable(testexeclp test/execlp.c)
add_executable(testexecv test/execv.c)
add_executable(testexecvp test/execvp.c)
#add_executable(hook_exec src/hook.c)
add_library(hookexecfamily SHARED src/hook.c)
target_link_libraries(hookexecfamily dl)
