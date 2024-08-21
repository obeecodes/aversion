#pragma once

#include <SDL2/SDL.h>
#include "../components/vec2.h"

struct TransformComponent{
    Vec2 position;
    int rotation;
    Vec2 scale;
    TransformComponent();
    TransformComponent(Vec2 position, int rotation = 0, Vec2 scale = Vec2{1, 1});
};