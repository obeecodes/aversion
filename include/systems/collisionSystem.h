#pragma once
#include "../components/colliderComponent.h"
#include "../systems/quadtree.h"
#include <vector>
#include "../core/ecs.h"
#include "../components/vec2.h"
#include <SDL2/SDL.h>
#include "eventManager.h"

class CollisionSystem{
public:
  static CollisionSystem* Instance();
  void update();
  void createColliderComponent(ColliderComponent collider, Uint32 entityID);
  std::vector<ColliderComponent>& getColliders();
  void createSubscriptions();
  
  void updateCollider(int x, int y, int width, int height);
  bool isCollision(SDL_Rect a, SDL_Rect b);
  SDL_Rect getSweptRect(SDL_Rect& rect, int xVel, int yVel);
  Vec2 getCollisionNormal(SDL_Rect& a, SDL_Rect& b);
  Quadtree* quadtree;

private:
  static CollisionSystem* instance;
  Aurora::ECS<ColliderComponent>* colliderComponentSystem;
  std::vector<ColliderComponent>& colliders;
  Uint32& inactiveIndex;
  EventManager* eventManager;
  CollisionSystem();

};
