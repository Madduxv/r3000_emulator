cmake_minimum_required(VERSION 3.30)
project(R3000_Emu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Enable compile_commands.json

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(r3000_emu ${SOURCES})

target_include_directories(r3000_emu PRIVATE include)
