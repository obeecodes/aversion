#pragma once
#include "../components/cameraComponent.h"
#include "../components/transformComponent.h"
#include <vector>
#include "../core/ecs.h"
#include <SDL2/SDL.h>
#include "eventManager.h"

class CameraSystem{
public:
  static CameraSystem* Instance();
  void update();
  void createCameraComponent(CameraComponent camera, Uint32 entityID);
  CameraComponent getCamera();
  void zoomIn();
  void zoomOut();

private:
  static CameraSystem* instance;
  Aurora::ECS<CameraComponent>* cameraComponentSystem;
  std::vector<CameraComponent>& cameras;
  Uint32& inactiveIndex;
  CameraSystem();
  Uint8 cameraScale;
  void createSubscriptions();
  void setActiveEntity(Uint32 entityID);

  EventManager* eventManager;

  Uint32 activeEntity;

};
