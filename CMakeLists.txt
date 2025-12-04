# Proyect setup:
cmake_minimum_required(VERSION 3.15)
project(CrraBloom)

# Crete the export command file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CFILE_PATH "src/LayoutNk.c")

# Custon flags:
option(BOX2D_TEST "Added box2d!" OFF)
option(SANITIZERS "Added Sanitizers" OFF)

# NOTE: Add a SANITIZERS flag if you want sanitizers for debuging and else
# But NO if you are building a realese!
if(SANITIZERS)
    message("Sanitizers ON! :)")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
else()
    message("Sanitizers OFF! :(")
endif()

# Include our modules from the the Include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Includes)

# Raylib:
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(raylib)

add_executable(${PROJECT_NAME} ${CFILE_PATH})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Box2d
if(BOX2D_TEST)
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
    message("Added Box2D!, remember that this flag is a test and later will be gone.")
else()
    message(STATUS "Box2D flag not set — only using Raylib.")
endif()

# Web config
if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    target_link_options(${PROJECT_NAME} PRIVATE
        "-sWASM=1"
        "-sASYNCIFY"
        "-sALLOW_MEMORY_GROWTH=1"
    )
endif()
