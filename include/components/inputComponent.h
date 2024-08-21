#pragma once 
#include <SDL2/SDL.h>
#include <vector>
#include <utility>

struct InputComponent{
    bool moveUp;
    bool moveDown;
    bool moveLeft;
    bool moveRight;
    bool createEntity;
    InputComponent(bool moveUp = false, bool moveDown = false, bool moveLeft = false, bool moveRight = false, bool createEntity = false);
};