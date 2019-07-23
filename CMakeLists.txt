cmake_minimum_required(VERSION 3.5)

project (paste_light)

file(GLOB SOURCES "src/*.cpp")

add_executable(paste_light ${SOURCES})

target_include_directories(paste_light
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/libs/maddy/include
)
