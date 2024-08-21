#pragma once 

#include "../components/stateComponent.h"
#include <vector>
#include <map>
#include <SDL2/SDL.h>
#include "../core/ecs.h"
#include "eventManager.h"

class StateSystem{
public:
  static StateSystem* Instance();
  void update();
  void createStateComponent(StateComponent state, Uint32 entityID);
  void initState();

private:
  static StateSystem* instance;
  Aurora::ECS<StateComponent>* stateComponentSystem;
  std::vector<StateComponent>& states;
  std::map<std::string, State> stringToState;

  Uint32& inactiveIndex;
  StateSystem();
  EventManager* eventManager;

};