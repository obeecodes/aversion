#pragma once 

#include <SDL2/SDL_stdinc.h>

struct HealthComponent{
    Uint32 max;
    Uint32 current;
    HealthComponent(Uint32 max = 20);
};
