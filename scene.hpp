#pragma once
#include <GL/gl3w.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace Scene {

struct FrameContext {
  const SDL_Event &event;
  const int windowWidth;
  const int windowHeight;
  const float frameTime;
};

void Init();
void CleanUp();
void DoFrame(const FrameContext &ctx);
void DoUI();
} // namespace Scene
