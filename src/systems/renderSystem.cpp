#include "../../include/core/renderer.h"
#include <SDL2/SDL_render.h>
#include "../../include/systems/aiSystem.h"
#include "../../include/systems/cameraSystem.h"
#include "../../include/systems/collisionSystem.h"
#include "../../include/systems/dialogueSystem.h"
#include "../../include/systems/healthSystem.h"
#include "../../include/systems/inputSystem.h"
#include "../../include/systems/lightSystem.h"
#include "../../include/systems/physicsSystem.h"
#include "../../include/systems/renderSystem.h"
#include "../../include/systems/spriteSystem.h"

RenderSystem* RenderSystem::instance = nullptr;

RenderSystem* RenderSystem::Instance(){
  if(!instance){
    instance = new RenderSystem();
  }
  return instance;
}

RenderSystem::RenderSystem(){
}

// movement should be performed first
// collision detection next
// camera location follows
// health depends of physics
// light system affects sprite
// render sprites
// dialogue is overlayed
void RenderSystem::update(){
/*
  InputSystem::Instance()->update();

  AISystem::Instance()->update();
  PhysicsSystem::Instance()->update();
  CollisionSystem::Instance()->update();
  CameraSystem::Instance()->update();
  HealthSystem::Instance()->update();
  LightSystem::Instance()->update();
  SpriteSystem::Instance()->update();
  DialogueSystem::Instance()->update();

  Renderer::Instance()->renderPresent();
  */
}
