#pragma once

#include <SDL2/SDL.h>
#include "vec2.h"

enum class ColliderType {
    STATIC,
    DYNAMIC,
    KINEMATIC,
    AUTOMATIC,
};

struct ColliderComponent{
    SDL_Rect rect;
    Vec2 offset;
    ColliderType type;

    ColliderComponent(SDL_Rect rect, Vec2 offset, ColliderType type = ColliderType::KINEMATIC);
    ColliderComponent(SDL_Rect rect, ColliderType type = ColliderType::KINEMATIC);
    ColliderComponent(SDL_Rect rect = SDL_Rect{});

};