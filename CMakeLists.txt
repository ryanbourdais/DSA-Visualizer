cmake_minimum_required(VERSION 3.20)

project(DSA-Visualizer VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

find_package(GLEW CONFIG REQUIRED)

add_executable(
    DSA-Visualizer
    main.c
    data_structures/int_list.c
)

target_include_directories(
    DSA-Visualizer
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/external/nuklear"
)

target_link_libraries(
    DSA-Visualizer
    PRIVATE
        glfw
        GLEW::GLEW
        opengl32
)