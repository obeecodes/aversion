#include "../../include/systems/cameraSystem.h"
#include "../../include/systems/physicsSystem.h"
#include "../../include/systems/spriteSystem.h"
#include "../../include/core/map.h"
#include "../../include/core/renderer.h"
#include "../../include/core/window.h"
#include <iostream>

#define ENTITY_WIDTH 32
#define ENTITY_HEIGHT 32

CameraSystem* CameraSystem::instance = nullptr;

CameraSystem* CameraSystem::Instance(){
  if(!instance){
    instance = new CameraSystem();
  }
  return instance;
}

CameraSystem::CameraSystem()
: cameraComponentSystem(new Aurora::ECS<CameraComponent>()),
  cameras(cameraComponentSystem->getComponents()),
  inactiveIndex(cameraComponentSystem->getInactiveIndex()),
  eventManager(EventManager::Instance()){

    createSubscriptions();

    this->activeEntity = 0; //entityID: player
    this->cameraScale = 4;

}

void CameraSystem::update(){
  setActiveEntity(this->activeEntity);
  for(Uint32 i = 0; i < this->inactiveIndex; i++){
    break;
    SDL_Rect viewport;
    SDL_RenderGetViewport(Renderer::Instance()->getRenderer(), &viewport);
    
    this->cameras[0].position.setX((644  + ENTITY_HEIGHT / 2) - viewport.w / 2);
    this->cameras[0].position.setY((644  + ENTITY_WIDTH / 2) - viewport.h / 2);
    
    if(this->cameras[0].position.getX() < 0){
      this->cameras[0].position.setX(0);
    }
    if(this->cameras[0].position.getY() < 0){
      this->cameras[0].position.setY(0);
    }

    if(this->cameras[0].position.getX() > Map::Instance()->getMapWidth() * 32 - viewport.w){
      this->cameras[0].position.setX(Map::Instance()->getMapWidth() * 32 - viewport.w);
    }

    if(this->cameras[0].position.getY() > Map::Instance()->getMapHeight() * 32 - viewport.h){
      this->cameras[0].position.setY(Map::Instance()->getMapHeight() * 32 - viewport.h);
    }
  }
}

void CameraSystem::createCameraComponent(CameraComponent cameracomponent, Uint32 entityID){
  this->cameraComponentSystem->createComponent(cameracomponent, entityID);
}

void CameraSystem::setActiveEntity(Uint32 entityID){
  this->activeEntity = entityID;
}

CameraComponent CameraSystem::getCamera(){
  return this->cameras[0];
}

void CameraSystem::zoomIn(){
  ++cameraScale;
  SDL_RenderSetIntegerScale(Renderer::Instance()->getRenderer(), SDL_TRUE);
  SDL_RenderSetScale(Renderer::Instance()->getRenderer(), cameraScale, cameraScale);
  SDL_RenderSetViewport(Renderer::Instance()->getRenderer(), NULL);

}

void CameraSystem::zoomOut(){
 if(cameraScale < 2) return;
  --cameraScale;
  SDL_RenderSetIntegerScale(Renderer::Instance()->getRenderer(), SDL_TRUE);
  SDL_RenderSetScale(Renderer::Instance()->getRenderer(), cameraScale, cameraScale);
  SDL_RenderSetViewport(Renderer::Instance()->getRenderer(), NULL);

}

void CameraSystem::createSubscriptions(){
  eventManager->subscribe(EventType::CAMERA_UPDATE, ([this](const Event event){
    SDL_Rect viewport;
    SDL_RenderGetViewport(Renderer::Instance()->getRenderer(), &viewport);
    
    this->cameras[0].position.setX((std::get<Vec2>(event.data).x  + ENTITY_HEIGHT / 2) - viewport.w / 2);
    this->cameras[0].position.setY((std::get<Vec2>(event.data).y  + ENTITY_WIDTH / 2) - viewport.h / 2);
    
    if(this->cameras[0].position.getX() < 0){
      this->cameras[0].position.setX(0);
    }
    if(this->cameras[0].position.getY() < 0){
      this->cameras[0].position.setY(0);
    }

    if(this->cameras[0].position.getX() > Map::Instance()->getMapWidth() * 32 - viewport.w){
      this->cameras[0].position.setX(Map::Instance()->getMapWidth() * 32 - viewport.w);
    }

    if(this->cameras[0].position.getY() > Map::Instance()->getMapHeight() * 32 - viewport.h){
      this->cameras[0].position.setY(Map::Instance()->getMapHeight() * 32 - viewport.h);
    }

  }));
}

