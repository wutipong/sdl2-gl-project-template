cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

# set the project name
project(sdl2-gl-project-template)

# add the executable
add_executable(main 
	"main.cpp" 
	"scene.hpp" "scene.cpp" 
	"io_util.hpp"
    "shader.hpp" "shader.cpp")

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

target_link_libraries(main PRIVATE 
    SDL2::SDL2 
    SDL2::SDL2main 
    imgui::imgui 
    unofficial::gl3w::gl3w 
    spdlog::spdlog 
    spdlog::spdlog_header_only
    fmt::fmt
    fmt::fmt-header-only)

target_compile_features(main PRIVATE cxx_std_20)
set_property(TARGET main PROPERTY CXX_STANDARD 20)
set_property(TARGET main PROPERTY CXX_STANDARD_REQUIRED ON)

if (MSVC)
	set_property(TARGET main PROPERTY WIN32_EXECUTABLE ON)
	target_compile_options(main PRIVATE /Zc:__cplusplus)
endif()

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "shaders/*.frag"
    "shaders/*.vert"
)

add_custom_target(SHADER_COMPILE 
    SOURCES ${GLSL_SOURCE_FILES} 
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy ${GLSL_SOURCE_FILES} "${CMAKE_CURRENT_BINARY_DIR}/shaders/"
)

add_dependencies(main SHADER_COMPILE)
