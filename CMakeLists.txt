cmake_minimum_required(VERSION 3.25)

project(wallshader
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_executable(wallshader
    src/main.cpp
)

target_link_libraries(wallshader
    PRIVATE
        Vulkan::Vulkan
        glfw
)

target_include_directories(wallshader
    PRIVATE
        include
)

target_compile_options(wallshader
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
