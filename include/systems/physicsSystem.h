#pragma once

#include "../components/physicsComponent.h"
#include "../components/transformComponent.h"
#include "../components/vec2.h"
#include "../systems/quadtree.h"
#include <vector>
#include <SDL2/SDL.h>
#include "../core/ecs.h"
#include "eventManager.h"

class PhysicsSystem{
public:
  static PhysicsSystem* Instance();
  void update();
  void createPhysicsComponent(PhysicsComponent physics, Uint32 entityID);
  void createTransformComponent(TransformComponent transform, Uint32 entityID);
  void undoMove(Vec2 v);
  void createSubscriptions();
  Uint32 getInactiveIndex();

  std::vector<TransformComponent>& getTransforms();
  std::vector<PhysicsComponent>& getPhysics();

  Aurora::ECS<PhysicsComponent>* getPhysicsComponentSystem();
  Aurora::ECS<TransformComponent>* getTransformComponentSystem();

private:
  static PhysicsSystem* instance;
  Aurora::ECS<PhysicsComponent>* physicsComponentSystem;
  std::vector<PhysicsComponent>& physics;
  Aurora::ECS<TransformComponent>* transformComponentSystem;
  std::vector<TransformComponent>& transforms;
  Uint32& inactiveIndex;

  PhysicsSystem();
  EventManager* eventManager;

};
