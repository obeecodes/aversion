#pragma once

#include "gameState.h"

class PauseState : public GameState {
public:
    void update() override;
    void render() override;

    PauseState();
private:
    
};