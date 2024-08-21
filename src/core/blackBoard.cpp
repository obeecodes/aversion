#include "../../include/core/blackBoard.h"
#include "../../include/systems/physicsSystem.h"

BlackBoard* BlackBoard::instance = nullptr;
Vec2 BlackBoard::playerPosition = Vec2{};

BlackBoard*BlackBoard::Instance(){
    if(!instance){
        instance = new BlackBoard();
    }
    return instance;
}

void BlackBoard::createSubscriptions(){eventManager->subscribe(EventType::PLAYER_MOVEMENT, [this](const Event event){
    playerPosition = std::get<Vec2>(event.data);
  });
}

void BlackBoard::initBlackBoard(){}

BlackBoard::BlackBoard()
:   eventManager(EventManager::Instance()){
    createSubscriptions();
}