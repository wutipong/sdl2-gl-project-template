#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <imgui.h>

#include "imgui_impl/imgui_impl_opengl3.h"
#include "imgui_impl/imgui_impl_sdl.h"

constexpr int WindowWidth = 800;
constexpr int WindowHeight = 600;

constexpr char GlslVersion[] = "#version 450";
constexpr int GlMajorVersion = 4;
constexpr int GlMinorVersion = 5;

constexpr glm::vec4 ClearColor = {0.33f, 0.67f, 1.0f, 1.00f};

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_GL_SetAttribute(
      SDL_GL_CONTEXT_FLAGS,
      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac4

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GlMajorVersion);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GlMinorVersion);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_Window *window = SDL_CreateWindow(
      "sdl2-gl-project-template", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED, WindowWidth, WindowHeight,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);

  SDL_SetWindowMinimumSize(window, WindowWidth, WindowHeight);

  SDL_GLContext glCtx = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, glCtx);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  gl3wInit();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();

  ImGui_ImplSDL2_InitForOpenGL(window, glCtx);
  ImGui_ImplOpenGL3_Init(GlslVersion);

  while (true) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        break;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    // Begin Draw UI
    ImGui::Begin("Hello");
    ImGui::LabelText("Hello", "Hello World");
    ImGui::End();
    // End Draw UI

    ImGui::EndFrame();
    ImGui::Render();

    int actualWidth, actualHeight;
    SDL_GetWindowSize(window, &actualWidth, &actualHeight);

    glViewport(0, 0, actualHeight, actualHeight);
    glDisable(GL_SCISSOR_TEST);
    glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw here
    /*
     *
     *
     *
     *
     */
    // end draw

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);
    SDL_Delay(1);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(glCtx);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
