#pragma once

#include "gameState.h"

class PlayState : public GameState {
public:
    void update() override;
    void render() override;

    PlayState();
    
private:
    
};