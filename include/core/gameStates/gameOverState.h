#pragma once

#include "gameState.h"

class GameOverState : public GameState {
public:
    void update();
    void render();

    GameOverState();
private:
    
};