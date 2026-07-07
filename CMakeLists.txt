cmake_minimum_required(VERSION 3.16)
project(SnakeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(snake
    src/main.cpp
    src/snake_game.cpp
)

target_include_directories(snake PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(snake PROPERTIES OUTPUT_NAME "snake")
