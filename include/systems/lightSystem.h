#pragma once
#include "../components/lightComponent.h"
#include <vector>
#include "../core/ecs.h"
#include <SDL2/SDL.h>
#include "eventManager.h"

class LightSystem{
public:
  static LightSystem* Instance();
  void update();
  void createLightComponent(LightComponent light, Uint32 entityID);

private:
  static LightSystem* instance;
  Aurora::ECS<LightComponent>* lightComponentSystem;
  std::vector<LightComponent>& lights;
  Uint32& inactiveIndex;
  LightSystem();
  EventManager* eventManager;

};
