#include "../../include/systems/healthSystem.h"

HealthSystem* HealthSystem::instance = nullptr;

HealthSystem* HealthSystem::Instance(){
  if(!instance){
    instance = new HealthSystem();
  }
  return instance;
}

HealthSystem::HealthSystem()
: healthComponentSystem(new Aurora::ECS<HealthComponent>()),
  healths(healthComponentSystem->getComponents()),
  inactiveIndex(healthComponentSystem->getInactiveIndex()),
  eventManager(EventManager::Instance()){

}

void HealthSystem::update(){
  for(Uint32 i = 0; i < this->inactiveIndex; i++){
    if(i == 0)
      eventManager->publish(Event{EventType::UPDATE_PLAYER_HEALTH, 0, this->healths[0]});
  }
}

void HealthSystem::createHealthComponent(HealthComponent health, Uint32 entityID){
  this->healthComponentSystem->createComponent(health, entityID);
}
