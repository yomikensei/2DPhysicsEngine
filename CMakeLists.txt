cmake_minimum_required(VERSION 3.27)

project(2DPhysicsEngine)

message("")
message(STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
message(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
message(STATUS "CMAKE_COMMAND: " ${CMAKE_COMMAND})
message(STATUS "CMAKE_VERSION: " ${CMAKE_VERSION})
message(STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER})
message(STATUS "CMAKE_PROJECT_NAME: " ${CMAKE_PROJECT_NAME})
message(STATUS "CMAKE_FINDCMAKE: " ${CMAKE_SOURCE_DIR}/CMAKE)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMAKE")

set(SOURCE_FILES src/main.cpp
        src/Application.cpp
        src/Application.h
        src/Graphics.cpp
        src/Graphics.h
        src/Physics/Vec2.cpp
        src/Physics/Vec2.h
        src/Physics/Particle.cpp
        src/Physics/Particle.h
        src/SDL2_gfx/SDL2_gfxPrimitives.c
        src/SDL2_gfx/SDL2_gfxPrimitives.h
        src/SDL2_gfx/SDL2_gfxPrimitives_font.h
        src/SDL2_gfx/SDL2_rotozoom.c
        src/SDL2_gfx/SDL2_rotozoom.h
        src/Physics/Constants.h
        src/Physics/Force.cpp
        src/Physics/Force.h
        src/Physics/Shape.cpp
        src/Physics/Shape.h
        src/Physics/Body.cpp
        src/Physics/Body.h
)


message("")
message(STATUS "SOURCE entry point : " ${SOURCE_FILES})
message("")

find_package(SDL2 REQUIRED SDL2)
find_package(SDL2_IMAGE REQUIRED SDL2_IMAGE)


add_executable(2DPhysicsEngine ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
        PRIVATE SDL2::SDL2
        PRIVATE SDL2_image::SDL2_image
)

