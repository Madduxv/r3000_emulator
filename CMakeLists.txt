cmake_minimum_required(VERSION 3.30)
project(R3000_Emu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Shared library from your src code
file(GLOB_RECURSE SRC_SOURCES "src/*.cpp")
add_library(r3000_core STATIC ${SRC_SOURCES})
target_include_directories(r3000_core PUBLIC include)

# Main executable
add_executable(r3000_emu src/main.cpp)
target_link_libraries(r3000_emu PRIVATE r3000_core)

# ------------------------------------
# Test setup
# ------------------------------------
enable_testing()

file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
add_executable(run_tests ${TEST_SOURCES})
target_include_directories(run_tests PRIVATE include)
target_link_libraries(run_tests PRIVATE r3000_core)

add_test(NAME RunAllTests COMMAND run_tests)

# cmake_minimum_required(VERSION 3.30)
# project(R3000_Emu)
#
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#
# file(GLOB_RECURSE SOURCES "src/*.cpp")
#
# add_executable(r3000_emu ${SOURCES})
#
# target_include_directories(r3000_emu PRIVATE include)
