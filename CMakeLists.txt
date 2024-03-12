cmake_minimum_required(VERSION 3.27)
project(pong_the_game C)

set(CMAKE_C_STANDARD 23)
add_executable(pong_the_game main.c
        common.h
        common.c)
message("[TOOLCHAIN] "  ${CMAKE_TOOLCHAIN_FILE})

target_compile_options(pong_the_game PRIVATE -Wall -Wextra -fsanitize=address)
target_link_options(pong_the_game PRIVATE -fsanitize=address)

find_package(raylib CONFIG REQUIRED)
if (APPLE)
    message("[APPLE] "  ${CMAKE_SYSTEM_NAME})
    target_link_libraries(pong_the_game PRIVATE raylib "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
else()
    target_link_libraries(pong_the_game PRIVATE raylib)
endif()

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(pong_the_game PRIVATE glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(pong_the_game PRIVATE glad::glad)
