# sdl2-gl-project-template

`sdl2-gl-project-template` is a C++ template project which configured for OpenGL and SDL2. This template comes with the following libraries preconfigured.

* [Simple DirectMedia Layer 2](http://libsdl.org/)
* [Dear Imgui](https://github.com/ocornut/imgui)
* [gl3w: Simple OpenGL core profile loading](https://github.com/skaslev/gl3w)
* [OpenGL Mathematics](https://github.com/g-truc/glm)

Above libraries are configured using [Vcpkg](https://github.com/microsoft/vcpkg) Library Manager and [Cmake](https://cmake.org/). 

## Usage

1. Clone this repository.
2. Remove the `origin` remote of your project so it does not point to the template repository.
3. Update the file `vcpkg.json` and `CMakeLists.txt` file to change the project name.

You may also want to set the new `origin` remote url, and may be squash the commit history so it does not contains any history of the template project.

Also since we use `Vcpkg` as the package management, you can also add new dependencies by updating the `vcpkg.json`. For more information please visit [Vcpkg](https://github.com/microsoft/vcpkg) homepage.

## Build the project

### Using IDE

Visual Studio 2019 and Visual Studio Code supports CMake project directly, given that the right components are installed. Simply use `Open Folder` menu to open the project file inside.

### Manually configure
* install `cmake`.
* run `cmake <project directory>` inside an empty directory to configure the project. The configure step might take some time as it build the dependencies projects at the fast time it is configured. 
* run `cmake --build .` (`.` must be included in the command) to build the project.

## Working with the project

Inside the file `main.cpp`, there are two commented sections. `//Begin UI ... //End UI` defines where to put `ImGUI`-based user interface code in, and `//Begin Draw ... //End Draw` designate where to put your drawing code in.

Also the same file, the variable `event` will be populated at the begining of the main loop. You can use this variable if you have to handle input events.

The project is configured to uses OpenGL 4.5. In the case that you have to work with different version, change the value of `GlMajorVersion`, `GlMinorVersion` and `GlslVersion` to the version you are using.

The `shaders` directory is where you're supposed to put shader files in. Any files in this directory will be copied to the build directory. This is preconfigured inside the `CMakeLists.txt` and can be changed or removed.

Last but not least, in the case that you need to add more file into the project, update the `CMakeLists.txt` file to accomodate those new files. If you're using VS2009, the IDE will be doing that for you (unless it can't figure out how).

## NOTICE
This project contains files taken from `Dear ImGui` project, which is also released under MIT license.
