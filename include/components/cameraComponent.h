#pragma once
#include <SDL2/SDL.h>
#include "../components/vec2.h"

struct CameraComponent{
  Vec2 position;
  int width;
  int height;
  CameraComponent();
  CameraComponent(Vec2 position, int width, int height);
};
