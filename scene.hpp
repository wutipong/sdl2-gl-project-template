#pragma once
#include <SDL2/SDL.h>

class Scene {
public:
  void Init();
  void CleanUp();
  void DoFrame(SDL_Event& event);
  void DoUI();
};
