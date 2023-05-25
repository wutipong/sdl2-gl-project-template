# sdl2-gl-project-template

`sdl2-gl-project-template` is a C++ template project which configured for OpenGL and SDL2. This template comes with the following libraries preconfigured.

* [Simple DirectMedia Layer 2](http://libsdl.org/)
* [Dear Imgui](https://github.com/ocornut/imgui)
* [gl3w: Simple OpenGL core profile loading](https://github.com/skaslev/gl3w)
* [OpenGL Mathematics](https://github.com/g-truc/glm)
* [Spdlog](https://github.com/gabime/spdlog)
* [{fmt}](https://github.com/fmtlib/fmt)
* [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
* [stb](https://github.com/nothings/stb)
* [glslang](https://github.com/KhronosGroup/glslang)

Above libraries are configured using [Vcpkg](https://github.com/microsoft/vcpkg) Library Manager and [Cmake](https://cmake.org/). 

Of course, feel free to add and/or remove library to use withing your project.

This template comes with GitHub actions to create build when changes are push back to GitHub. These actions create a build for Windows, Linux, and MacOS when main branch is updated, and a Windows build when any other branch is updated.

## Usage

### Using Github template feature.

1. Clone this repository into your Github account.
2. Create a new project on Github with this project template.
3. Don't forget to update this README file.

Also since we use `Vcpkg` as the package management, you can also add new dependencies by updating the `vcpkg.json`. For more information please visit [Vcpkg](https://github.com/microsoft/vcpkg) homepage.

## Build the project

### Using IDE

Visual Studio 2022 and Visual Studio Code supports CMake project directly, given that the right components are installed. Simply use `Open Folder` menu to open the project file inside.

### Manually configuring
* install `cmake`.
* create an empty directory as the build directory.
* run `cmake <project directory>` inside the build to configure the project. The configure step might take some time as it build the dependencies projects at the fast time it is configured. 
* run `cmake --build <build directory>` (`.` must be included in the command) to build the project.

## Working with the project

The provided source code is a very basic barebone code to run a simply OpenGL program. Please feel free to change any of these to suit your needs.

### `Scene` namespace
This namespace contains the application logic, separated from the aplication initialization code and boilerplates.

To make your own application from this template, update the file `scene.cpp` to meet your requirement. 

* `Init()` contains the initialization code.
* `DoFrame()` contains code that run each time a new frame is rendering.
* `DoUI()` contains the DearImGui code to perform user interface.
* `CleanUp()` contains the clean-up code.

Also if you don't need Dear ImGui-based UI, change the value of `HasUI` variable inside `scene.hpp` to `false`.

If you're not going to use the `ImGui` UI at all, please feel free to remove the relevant code out of the project.

The project is configured to uses OpenGL 4.5. In the case that you have to work with different version, change the value of `GlMajorVersion`, `GlMinorVersion` and `GlslVersion` to the version you are using.

### `IOUtiliy` namespace

This file contains a very basic file reading function `LoadFile()`. Basically it read the whole file and placed it into the supplied container, usually a `vector`. By default, the file is read as a text file, but additional `std::ios_base::openmode` can be used to cchange the way the file is read, eg `std::ios::binary` for reading binary file.

### `Shader` namespace

This namespace contains functions to load shader file, both in source file and binary (SPIR-V). It also contains a utility function to link shaders to a program with error checking and link logging etc.

### `Texture` namespace

This namespace contains a function to load a texture file into `TEXTURE_2D` target. The output texture is always 4-channel (RGBA) regardless of the source file.

### `CMakeLists.txt`

In the `CMakeLists.txt` file, there are two additional functions `copy_resources` and `compile_shaders`. These can be used to copy resources and compile shaders to the output. Please see the example inside the file.