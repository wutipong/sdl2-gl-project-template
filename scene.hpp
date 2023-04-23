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
  glm::vec4 lightDir{0, -1.0f, 0, 0};
  float ambient = 0.10f;

  GLuint program;
  GLuint vertShader;
  GLuint fragShader;

  GLuint vao;
  GLuint vbo;

  // clang-format off
  static constexpr float vertices[] = {
    /* position      **  normal        */
    -1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,
     1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,
    -1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,
     1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,
     1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,
     1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,
    -1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
     1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,
    -1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,
    -1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
     1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
     1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
     1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
     1.0f, 1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
     1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
     1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
     1.0f,-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
     1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
     1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,
     1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
     1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
     1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
  };
  // clang-format on

  static constexpr int componentPerVector = 3;
  static constexpr int componentPerVertex = 2 * componentPerVector;
  static constexpr int componentCount = sizeof(vertices) / sizeof(float);
  static constexpr int vertexCount = componentCount / componentPerVertex;
  static constexpr int triangleCount = vertexCount / 3;
};
