#pragma once

#include <SDL2/SDL.h>
#include "../components/vec2.h"

struct PhysicsComponent{
  Vec2 velocity;
  Vec2 acceleration;

  PhysicsComponent();
  PhysicsComponent(Vec2 velocity, Vec2 acceleration);

};
