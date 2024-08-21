#include "../../include/systems/lightSystem.h"

LightSystem* LightSystem::instance = nullptr;

LightSystem* LightSystem::Instance(){
  if(!instance){
    instance = new LightSystem();
  }
  return instance;
}

LightSystem::LightSystem()
: lightComponentSystem(new Aurora::ECS<LightComponent>()),
  lights(lightComponentSystem->getComponents()),
  inactiveIndex(lightComponentSystem->getInactiveIndex()),
  eventManager(EventManager::Instance()){

}

void LightSystem::update(){
  for(Uint32 i = 0; i < this->inactiveIndex; i++){

  }
}

void LightSystem::createLightComponent(LightComponent light, Uint32 entityID){
  this->lightComponentSystem->createComponent(light, entityID);
}
