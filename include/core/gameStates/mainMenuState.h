#pragma once

#include "gameState.h"
#include "../widgets/button.h"
#include <vector>
#include <chrono>

class  MainMenuState : public GameState {
public:
    void update() override;
    void render() override;

     MainMenuState();
    ~ MainMenuState();
private:
    std::vector<Button*> buttons;
    
};
