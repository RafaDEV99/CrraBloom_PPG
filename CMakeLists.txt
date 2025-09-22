cmake_minimum_required(VERSION 3.15)

# Proyect name
project(CrraBloom)

# Crete the export command file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include our modules from the the Include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Include)

add_executable(${PROJECT_NAME} test/test.c)

# Packages stuff :P
find_package(raylib REQUIRED)

# include(FetchContent)
# FetchContent_Declare(
# 	box2d
# 	GIT_REPOSITORY https://github.com/erincatto/box2d.git
# 	GIT_TAG main
# 	GIT_SHALLOW TRUE
# 	GIT_PROGRESS TRUE
# )
# FetchContent_MakeAvailable(box2d)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
