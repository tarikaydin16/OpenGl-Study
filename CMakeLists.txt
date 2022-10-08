cmake_minimum_required(VERSION 2.8)

project(OPENGL_DERS_4)

set(PROJECT_DIR ${CMAKE_SOURCE_DIR})

set(PROJECT_INCLUDE_DIR ${PROJECT_DIR}/include)
set(PROJECT_SOURCE_DIR ${PROJECT_DIR}/src)
# add_compile_options(-Wall -Wextra -pedantic -Werror -target x86_64-apple-darwin20.3.0)

# set(CMAKE_REQUIRED_LINK_OPTIONS "-arch;arm64")
# check_c_compiler_flag("-arch arm64" arm64Supported)
# message("arm64Supported=${arm64Supported}")


set(PROJECT_SOURCES 
                ${PROJECT_SOURCE_DIR}/glad.c
                ${PROJECT_SOURCE_DIR}/shaderprogram.cpp
                ${PROJECT_SOURCE_DIR}/main.cpp
                )


find_package(glfw3 3.2 REQUIRED)
find_package(glm REQUIRED)

include_directories(${PROJECT_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} glfw)