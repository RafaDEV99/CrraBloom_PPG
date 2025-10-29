cmake_minimum_required(VERSION 3.15)

# Proyect name
project(CrraBloom)

# Crete the export command file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Custon flags:
option(BOX2D "Added box2d!" OFF)
option(SANITIZERS "Added Sanitizers" OFF)

# Added sanitizers like:
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

# NOTE: Add a SANITIZERS flag if you want sanitizers for debuging and else
if(SANITIZERS) # <-- Temp may change
    message("-- Sanitizers ON! :)")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
else()
    message("-- Sanitizers OFF! :(")
endif()

# Include our modules from the the Include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Includes)

# Packages stuff
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

add_executable(${PROJECT_NAME} test/mainTest.c)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# TODO: Add Box2d and raylib
if(BOX2D)
    message("Adding Box2d and raylib!")
    include(FetchContent)
    FetchContent_Declare(
    	box2d
    	GIT_REPOSITORY https://github.com/erincatto/box2d.git
    	GIT_TAG main
    	GIT_SHALLOW TRUE
    	GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(box2d)
    target_link_libraries(${PROJECT_NAME} PRIVATE box2d)
else()
    message(STATUS "Box2D flag not set — only using Raylib.")
endif()

