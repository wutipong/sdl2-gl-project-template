#pragma once
union SDL_Event;

namespace Scene {
constexpr bool HasUI = true;

struct FrameContext {
  const SDL_Event &event;
  const int windowWidth;
  const int windowHeight;
  const float frameTime;
};

bool Init();
void CleanUp();
void DoFrame(const FrameContext &ctx);
void DoUI();
} // namespace Scene
