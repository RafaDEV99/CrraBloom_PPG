cmake_minimum_required(VERSION 3.15)

# Proyect name
project(CraBOOM_Eng)

# Crete the export command file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include our modules from the the Include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)

add_executable(${PROJECT_NAME} main.c)

# Packages stuff :P
find_package(raylib REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

