#pragma once
#include "../components/healthComponent.h"
#include <vector>
#include "../core/ecs.h"
#include <SDL2/SDL.h>
#include "eventManager.h"

class HealthSystem{
public:
  static HealthSystem* Instance();
  void update();
  void createHealthComponent(HealthComponent health, Uint32 entityID);

private:
  static HealthSystem* instance;
  Aurora::ECS<HealthComponent>* healthComponentSystem;
  std::vector<HealthComponent>& healths;
  Uint32& inactiveIndex;
  EventManager* eventManager;
  HealthSystem();

};
