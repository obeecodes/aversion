#pragma once

#include "../components/vec2.h"
#include "../systems/eventManager.h"

struct BlackBoard {
    static Vec2 playerPosition;
    // TransformComponent safeArea;
    // Instead of a position being the safe area, use a rect
    BlackBoard();
    void initBlackBoard();
    static BlackBoard* Instance();
    void createSubscriptions();
    static BlackBoard* instance;
    EventManager* eventManager;
};