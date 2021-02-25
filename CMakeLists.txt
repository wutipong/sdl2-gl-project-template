cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

# set the project name
project(sdl2-gl-project-template)

# add the executable
add_executable(sdl2-gl-project-template 
	"main.cpp" 
	"imgui_impl/imgui_impl_sdl.h" 
	"imgui_impl/imgui_impl_sdl.cpp"
	"imgui_impl/imgui_impl_opengl3.h" 
	"imgui_impl/imgui_impl_opengl3.cpp" 
	"scene.hpp" "scene.cpp" 
	"io_util.hpp"
 "shader.hpp" "shader.cpp")

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

target_link_libraries(sdl2-gl-project-template PRIVATE 
    SDL2::SDL2 
    SDL2::SDL2main 
    imgui::imgui 
    unofficial::gl3w::gl3w 
    spdlog::spdlog 
    spdlog::spdlog_header_only
    fmt::fmt)

target_compile_features(sdl2-gl-project-template PRIVATE cxx_std_17)

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "shaders/*.frag"
    "shaders/*.vert"
)

add_custom_target(SHADER_COMPILE 
    SOURCES ${GLSL_SOURCE_FILES} 
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy ${GLSL_SOURCE_FILES} "${CMAKE_CURRENT_BINARY_DIR}/shaders/"
)

add_dependencies(sdl2-gl-project-template SHADER_COMPILE)
