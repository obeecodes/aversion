#pragma once

#include "gameState.h"

class TransitionState : public GameState {
public:
    void update();
    void render();

    TransitionState();
private:
    
};