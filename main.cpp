#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <spdlog/spdlog.h>

#include "scene.hpp"
#include "shader.hpp"

constexpr char ProjectName[] = "sdl-gl-project-template";
constexpr int WindowWidth = 800;
constexpr int WindowHeight = 600;

constexpr char GlslVersion[] = "#version 410";
constexpr int GlMajorVersion = 4;
constexpr int GlMinorVersion = 5;

constexpr bool EnableGLDebug = false;

void PrintDeviceInformation();
void GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                     const void *userParam);

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_EVERYTHING);
  spdlog::set_level(spdlog::level::debug);

  spdlog::info("{} - starts.", ProjectName);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GlMajorVersion);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GlMinorVersion);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  if (EnableGLDebug) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
  }

  SDL_Window *window =
      SDL_CreateWindow("sdl2-gl-project-template", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WindowWidth,
                       WindowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);

  SDL_SetWindowMinimumSize(window, WindowWidth, WindowHeight);

  SDL_GLContext glCtx = SDL_GL_CreateContext(window);
  if (glCtx == nullptr) {
    spdlog::error("Error: {}.\n", SDL_GetError());

    return -1;
  }

  SDL_GL_MakeCurrent(window, glCtx);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  gl3wInit();

  PrintDeviceInformation();

  glDebugMessageCallback(GLDebugCallback, nullptr);

  if (Scene::HasUI) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL2_InitForOpenGL(window, glCtx);
    ImGui_ImplOpenGL3_Init(GlslVersion);
  }

  Scene::Init();

  auto lastFrame = SDL_GetTicks64();

  while (true) {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
      if (Scene::HasUI) {
        ImGui_ImplSDL2_ProcessEvent(&event);
      }

      if (event.type == SDL_QUIT)
        break;
    }

    if (Scene::HasUI) {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame(window);
      ImGui::NewFrame();

      Scene::DoUI();

      ImGui::EndFrame();
      ImGui::Render();
    }

    int actualWidth, actualHeight;
    SDL_GetWindowSize(window, &actualWidth, &actualHeight);

    auto now = SDL_GetTicks64();

    Scene::DoFrame({
        .event = event,
        .windowWidth = actualWidth,
        .windowHeight = actualHeight,
        .frameTime = ((float)now - (float)lastFrame) / 1000.0f,
    });

    lastFrame = now;

    if (Scene::HasUI) {
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    SDL_GL_SwapWindow(window);
    SDL_Delay(1);
  }

  Scene::CleanUp();

  if (Scene::HasUI) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }

  SDL_GL_DeleteContext(glCtx);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

void PrintDeviceInformation() {
  spdlog::info("OpenGL Device Information.");
  spdlog::info("\tOpenGL: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));
  spdlog::info("\tGLSL: {}", reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
  spdlog::info("\tDevice: {}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
  spdlog::info("\tVendor: {}", reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
  spdlog::info("Supported Extensions:");

  int extCount;
  glGetIntegerv(GL_NUM_EXTENSIONS, &extCount);

  for (int i = 0; i < extCount; i++) {
    spdlog::info("\t{}", reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i)));
  }
}

void GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                     const void *userParam) {

  std::string messageStr = message;

  spdlog::level::level_enum level;
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    level = spdlog::level::critical;
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    level = spdlog::level::err;
    break;
  case GL_DEBUG_SEVERITY_LOW:
    level = spdlog::level::warn;
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    level = spdlog::level::info;
    break;
  }

  std::string typeStr;
  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    typeStr = "error";
    break;

  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    typeStr = "deprecated behavior";
    break;

  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    typeStr = "undefined behavior";
    break;

  case GL_DEBUG_TYPE_PORTABILITY:
    typeStr = "portability";
    break;

  case GL_DEBUG_TYPE_PERFORMANCE:
    typeStr = "performance";
    break;

  case GL_DEBUG_TYPE_MARKER:
    typeStr = "marker";
    break;

  case GL_DEBUG_TYPE_PUSH_GROUP:
    typeStr = "push group";
    break;

  case GL_DEBUG_TYPE_POP_GROUP:
    typeStr = "pop group";
    break;

  case GL_DEBUG_TYPE_OTHER:
    typeStr = "other";
    break;
  }

  std::string sourceStr;
  switch (source) {
  case GL_DEBUG_SOURCE_API:
    sourceStr = "api";
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    sourceStr = "window system";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    sourceStr = "compiler";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    sourceStr = "third party";
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    sourceStr = "application";
    break;
  case GL_DEBUG_SOURCE_OTHER:
    sourceStr = "other";
    break;
  }

  spdlog::log(level, "[{:08d}] [{}] [{}] - {}", id, typeStr, sourceStr, messageStr);
}
