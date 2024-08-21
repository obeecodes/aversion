#include "../../../include/core/gameStates/playState.h"
#include <SDL2/SDL_stdinc.h>
#include "../../../include/systems/renderSystem.h"
#include "../../../include/systems/dialogueSystem.h"
#include "../../../include/systems/inputSystem.h"
#include "../../../include/core/renderer.h"
#include "../../../include/systems/aiSystem.h"
#include "../../../include/systems/audioSystem.h"
#include "../../../include/systems/cameraSystem.h"
#include "../../../include/systems/collisionSystem.h"
#include "../../../include/systems/healthSystem.h"
#include "../../../include/systems/lightSystem.h"
#include "../../../include/systems/physicsSystem.h"
#include "../../../include/entities/entityManager.h"
#include "../../../include/systems/spriteSystem.h"
#include "../../../include/systems/eventManager.h"
#include "../../../include/core/map.h"

#include <chrono>
/*
int count = 0;
const Uint64 FPS = 60;
const std::chrono::duration<float> CYCLE_TIME { 1.0f / FPS };

  ///
  auto system_timer_start = std::chrono::high_resolution_clock::now();
  auto system_timer_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed_seconds = system_timer_end - system_timer_start;
  float accumulated_seconds{ 0.0f };
  //
*/
  //Uint64 frameStart, frameTime;
PlayState::PlayState(){

}

void PlayState::update(){

  InputSystem::Instance()->update();
  AISystem::Instance()->update();
  EventManager::Instance()->processEvents();

  AudioSystem::Instance()->update();
  PhysicsSystem::Instance()->update();
  CollisionSystem::Instance()->update();
  //CameraSystem::Instance()->update();
  HealthSystem::Instance()->update();
  LightSystem::Instance()->update();
  DialogueSystem::Instance()->update();

  EventManager::Instance()->processEvents();
}

void PlayState::render(){
  Renderer::Instance()->renderClear();

  Map::Instance()->update();
  DialogueSystem::Instance()->update();
  SpriteSystem::Instance()->update();

  Renderer::Instance()->renderPresent();
}