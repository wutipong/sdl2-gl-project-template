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
find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(glslang CONFIG REQUIRED)

find_program(GLSL_VALIDATOR glslangValidator REQUIRED)

# target_link_libraries(main PRIVATE glslang::OSDependent glslang::glslang glslang::MachineIndependent glslang::GenericCodeGen glslang::glslang-default-resource-limits glslang::OGLCompiler glslang::SPVRemapper glslang::SPIRV glslang::HLSL)

target_link_libraries(main PRIVATE 
    SDL2::SDL2 
    SDL2::SDL2main 
    OpenGL::GL
    imgui::imgui
    imguizmo::imguizmo
    unofficial::gl3w::gl3w 
    spdlog::spdlog 
    spdlog::spdlog_header_only
    fmt::fmt
    fmt::fmt-header-only)

target_compile_features(main PRIVATE cxx_std_20)
set_property(TARGET main PROPERTY CXX_STANDARD 20)
set_property(TARGET main PROPERTY CXX_STANDARD_REQUIRED ON)

if (MSVC)
	# set_property(TARGET main PROPERTY WIN32_EXECUTABLE ON)
	target_compile_options(main PRIVATE /Zc:__cplusplus)
endif()

function(compile_shaders)
    set(oneValueArgs TARGET TARGET)
    set(multiValueArgs TARGET INPUT)
    set(oneValueArgs DESTINATION DESTINATION)

    cmake_parse_arguments(COMPILE_SHADERS "${options}" "${oneValueArgs}"
            "${multiValueArgs}" ${ARGN}
            )

    file(GLOB GLSL_SOURCE_FILES ${COMPILE_SHADERS_INPUT})
    get_filename_component(_dir "${CMAKE_CURRENT_BINARY_DIR}/${COMPILE_SHADERS_DESTINATION}" DIRECTORY)

    add_custom_target(${COMPILE_SHADERS_TARGET}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${_dir}
            COMMAND ${GLSL_VALIDATOR} -G -e main -l -o "${CMAKE_CURRENT_BINARY_DIR}/${COMPILE_SHADERS_DESTINATION}" -q ${GLSL_SOURCE_FILES}        
    )
    add_dependencies(main ${COMPILE_SHADERS_TARGET})
endfunction()

compile_shaders(
        TARGET CompileVertexShader
        DESTINATION "shaders/shader.vert.spv"
        INPUT "shaders/shader.vert"
)   

compile_shaders(
        TARGET CompileFragmentShader
        DESTINATION "shaders/shader.frag.spv"
        INPUT "shaders/shader.frag"
) 