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
	"imgui_impl/imgui_impl_opengl3.cpp" "scene.hpp" "scene.cpp" "io_util.hpp")

find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(sdl2-gl-project-template PRIVATE SDL2::SDL2 SDL2::SDL2main imgui::imgui unofficial::gl3w::gl3w)

target_compile_features(sdl2-gl-project-template PRIVATE cxx_std_17)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")