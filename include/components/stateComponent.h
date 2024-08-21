#pragma once

#include <SDL2/SDL_stdinc.h>

enum class State{
    IDLE,
    INTERACTING,
    WALKING,
    ATTACKING,
    HURT,
    DEAD,
};

struct StateComponent{
    State state;
    Uint8 timer;
    StateComponent(State state = State::IDLE);
};