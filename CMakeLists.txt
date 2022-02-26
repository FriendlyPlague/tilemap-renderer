cmake_minimum_required(VERSION 3.17)

project(sdlgame)

add_executable(${PROJECT_NAME} main.cpp renderer.cpp)

target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image)
