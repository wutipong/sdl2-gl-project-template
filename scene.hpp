#pragma once
#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

struct FrameContext {
  const SDL_Event &event;
  const int windowWidth;
  const int windowHeight;
  const float frameTime;
};

class Scene {
public:
  void Init();
  void CleanUp();
  void DoFrame(const FrameContext &ctx);
  void DoUI();

private:
  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

  GLuint program;
  GLuint vertShader;
  GLuint fragShader;

  GLuint vao;
  GLuint vbo;
};
