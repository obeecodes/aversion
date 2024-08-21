#include "../../include/systems/aiSystem.h"
#include "../../include/core/blackBoard.h"
#include "../../include/core/tsuki.h"

AISystem* AISystem::instance = nullptr;

AISystem* AISystem::Instance(){
    if(!instance){
        instance = new AISystem();
    }
    return instance;
}

AISystem::AISystem()
:   aiComponentSystem(new Aurora::ECS<AIComponent>()),
    ais(aiComponentSystem->getComponents()),
    inactiveIndex(aiComponentSystem->getInactiveIndex()){

}

void AISystem::createAIComponent(AIComponent ai, Uint32 entityID){
  this->aiComponentSystem->createComponent(ai, entityID);
}

void AISystem::update(){
    Tsuki::Instance()->pushVec2(BlackBoard::playerPosition);

    for(Uint32 i = 0; i < this->inactiveIndex; i++){
        Uint32 id = aiComponentSystem->getEntityID(i);
        Tsuki::Instance()->pushEntityId(id);
        Tsuki::Instance()->callScript(ais[i].script.c_str());
    }
}