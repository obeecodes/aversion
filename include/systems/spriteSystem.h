#pragma once

#include "../components/spriteComponent.h"
#include "../SDL2/SDL_image.h"
#include "../components/transformComponent.h"
#include "../components/inputComponent.h"
#include "../systems/cameraSystem.h"
#include <vector>
#include <unordered_map>
#include "../core/ecs.h"
#include <SDL2/SDL.h>
#include "../core/TMXParser.h"
#include "eventManager.h"

// Defined in spriteComponent.h
/*
enum Atlas{
  PLAYER = 1 << 0,
  WORLD = 1 << 1,
  UI = 1 << 2,
  ENEMY = 1 << 3,
  BLANK = 1 << 4,
};
*/

struct Bar{
  int r;
  int g;
  int b;
  int a;
  SDL_Rect rect;
  bool fill;
  Bar(int r, int g, int b, int a, SDL_Rect rect, bool fill = false): r(r), g(g), b(b), a(a), rect(rect), fill(fill){}
};

class SpriteSystem{
public:
  static SpriteSystem* Instance();
  void initTextures();
  void addTexture(Atlas atl, const char* file);
  void update();
  void createSpriteComponent(const std::string& entity, SDL_Rect src, SDL_Rect dest, Uint32 entityID);
  void createSpriteComponent(SpriteComponent sprite, Uint32 entityID);
  void destroySpriteComponent(Uint32 entityID);
  std::vector<SpriteComponent>& getSprites();
  Aurora::ECS<SpriteComponent>* getSpriteComponentSystem();
  std::unordered_map<Atlas, SDL_Texture*>& getTextures();
  SDL_Texture* getTexture(Atlas type);
  void createSubscriptions();
  Uint32 getInactiveIndex();



private:
  CameraComponent camera;
  static SpriteSystem* instance;
  std::unordered_map<Atlas, SDL_Texture*> textures;
  std::unordered_map<std::string, Atlas> stringToAtlas;
  std::vector<TransformComponent>& transforms;
  Aurora::ECS<SpriteComponent>* spriteComponentSystem;
  std::vector<SpriteComponent>& sprites;
  std::vector<SpriteComponent> sortedSprites;

  std::vector<Bar> bars;

  Uint32& inactiveIndex;
  SpriteSystem();
  EventManager* eventManager;

};
