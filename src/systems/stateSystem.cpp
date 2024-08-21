#include "../../include/systems/stateSystem.h"

StateSystem* StateSystem::instance = nullptr;

StateSystem* StateSystem::Instance(){
    if(!instance){
        instance = new StateSystem();
    }
    return instance;
}

StateSystem::StateSystem()
:   stateComponentSystem(new Aurora::ECS<StateComponent>()),
    states(stateComponentSystem->getComponents()),
    inactiveIndex(stateComponentSystem->getInactiveIndex()),
    eventManager(EventManager::Instance()){

}

void StateSystem::createStateComponent(StateComponent state, Uint32 entityID){
  this->stateComponentSystem->createComponent(state, entityID);
}

void StateSystem::initState(){
    stringToState["IDLE"] = State::IDLE;
    stringToState["INTERACTING"] = State::INTERACTING;
    stringToState["WALKING"] = State::WALKING;
    stringToState["ATTACKING"] = State::ATTACKING;
    stringToState["HURT"] = State::HURT;
    stringToState["DEAD"] = State::DEAD;
}


void StateSystem::update(){
    for(Uint32 i = 0; i < this->inactiveIndex; i++){
        
    }
}