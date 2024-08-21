#pragma once 

#include "../components/aiComponent.h"
#include <vector>
#include <SDL2/SDL.h>
#include "../core/ecs.h"

class AISystem{
public:
  static AISystem* Instance();
  void update();
  void createAIComponent(AIComponent ai, Uint32 entityID);

private:
  static AISystem* instance;
  Aurora::ECS<AIComponent>* aiComponentSystem;
  std::vector<AIComponent>& ais;
  Uint32& inactiveIndex;
  AISystem();

};