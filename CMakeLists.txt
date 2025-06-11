cmake_minimum_required(VERSION 3.10)

project(Kraftdinner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-Wall -DGLM_ENABLE_EXPERIMENTAL")

set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -g")
set(CMAKE_CXX_FLAGS_PROFILE "-pg")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(src lib)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp lib/imgui/*.cpp)

add_executable(Kraftdinner ${SOURCE_FILES})

target_link_libraries(Kraftdinner -lSDL2 -lSDL2_image -lGL -lGLEW)

add_custom_command(
    TARGET Kraftdinner POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res
    ${CMAKE_CURRENT_BINARY_DIR}/res
)

add_custom_command(
    TARGET Kraftdinner POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    ${CMAKE_CURRENT_BINARY_DIR}/shaders
)
