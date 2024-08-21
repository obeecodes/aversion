#pragma once

#include "vec2.h"

struct ParticleComponent {
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    float duration;

    ParticleComponent(Vec2 position = Vec2{}, Vec2 velocity = Vec2{}, Vec2 acceleration = Vec2{}, float duration = 0);
};