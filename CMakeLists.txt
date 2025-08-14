cmake_minimum_required(VERSION 3.15)

project(CraBOOM_Eng)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)
add_executable(${PROJECT_NAME} main.c)
find_package(raylib REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
