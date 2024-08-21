#include "../../include/systems/spriteSystem.h"
#include "../../include/systems/collisionSystem.h"
#include "../../include/systems/physicsSystem.h"
#include "../../include/systems/inputSystem.h"
#include "../../include/systems/cameraSystem.h"
#include "../../include/core/renderer.h"
#include "../../include/entities/entityManager.h"
#include <iostream>
#include <algorithm>

const int TILE_SIZE = 32;
static Uint32 rotation = 0;
#define PI 3.14159265358979323846

static int count = 0;

static float change = 0;

SpriteSystem* SpriteSystem::instance = nullptr;

auto isTile = [](const std::string& gid) -> bool{
  try{
    std::stoi(gid);
  } catch(const std::exception& e){
    return false;
  }
  return true;
};

SpriteSystem* SpriteSystem::Instance(){
  if(!instance){
    instance = new SpriteSystem();
  }
  return instance;
}

SpriteSystem::SpriteSystem() 
: spriteComponentSystem(new Aurora::ECS<SpriteComponent>()),
  sprites(spriteComponentSystem->getComponents()),
  inactiveIndex(spriteComponentSystem->getInactiveIndex()),
  transforms(PhysicsSystem::Instance()->getTransforms()),
  eventManager(EventManager::Instance()){
    this->camera = CameraSystem::Instance()->getCamera();
    createSubscriptions();
}

void SpriteSystem::initTextures(){

  this->textures.insert({PLAYER, IMG_LoadTexture(Renderer::Instance()->getRenderer(), "assets/images/player_atlas.png")});
  this->textures.insert({PUP, IMG_LoadTexture(Renderer::Instance()->getRenderer(), "assets/images/pup_atlas.png")});
  this->textures.insert({ENEMY, IMG_LoadTexture(Renderer::Instance()->getRenderer(), "assets/images/enemy_atlas.png")});
  this->textures.insert({WORLD, IMG_LoadTexture(Renderer::Instance()->getRenderer(), "assets/images/tile_atlas.png")});
  this->textures.insert({UI, IMG_LoadTexture(Renderer::Instance()->getRenderer(), "assets/images/ui_atlas.png")});
  this->textures.insert({RAIN, IMG_LoadTexture(Renderer::Instance()->getRenderer(), "assets/images/rain.png")});
  this->stringToAtlas["Player"] = PLAYER;
  this->stringToAtlas["Pup"] = PUP;
  this->stringToAtlas["World"] = WORLD;
  this->stringToAtlas["UI"] = UI;
  this->stringToAtlas["Enemy"] = ENEMY;
  this->stringToAtlas["Blank"] = BLANK;
  this->stringToAtlas["Rain"] = RAIN;

}

void SpriteSystem::addTexture(Atlas atl, const char* file){
  this->textures.insert({atl, IMG_LoadTexture(Renderer::Instance()->getRenderer(), file)});
}

void SpriteSystem::createSpriteComponent(const std::string& entity, SDL_Rect src, SDL_Rect dest, Uint32 entityID){
  this->spriteComponentSystem->createComponent(SpriteComponent{this->stringToAtlas[entity], src, dest}, entityID);
}

void SpriteSystem::createSpriteComponent(SpriteComponent sprite, Uint32 entityID){
  this->spriteComponentSystem->createComponent(sprite, entityID);
}


void SpriteSystem::destroySpriteComponent(Uint32 entityID){
  this->spriteComponentSystem->destroyComponent(entityID);
}

void SpriteSystem::update(){

  if(false && count % 120 == 0){
    count = 0;
    for(Uint32 i = 0; i < 50; i++){
      EntityManager::Instance()->createRain();
    }
  }

  count++;

  change += 5;

  sortedSprites.clear();
  for (size_t i = 0; i < inactiveIndex; ++i) {
      const SpriteComponent& sprite = sprites[i];
      auto it = std::lower_bound(sortedSprites.begin(), sortedSprites.end(), sprite, 
          [](const SpriteComponent& a, const SpriteComponent& b) {
              return a.dest_rect.y < b.dest_rect.y;
          });
      sortedSprites.insert(it, sprite);
  }

  this->camera = CameraSystem::Instance()->getCamera();

  //renders entity
  for(Uint32 i = 0; i < this->inactiveIndex; i++){

    int x = this->sortedSprites[i].dest_rect.x - this->camera.position.x;
    int y = this->sortedSprites[i].dest_rect.y - this->camera.position.y;
    int w = this->sortedSprites[i].dest_rect.w;
    int h = this->sortedSprites[i].dest_rect.h;

    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(Renderer::Instance()->getRenderer(), this->textures[this->sortedSprites[i].atl], &(this->sortedSprites[i].src_rect), &(rect));
    //SDL_RenderTextureRotated(Renderer::Instance()->getRenderer(), this->textures[this->sprites[i].atl], &(this->sprites[i].src_rect), &(rect), rotation, nullptr, SDL_FLIP_NONE);

/*
    SDL_Rect collider = CollisionSystem::Instance()->getColliders()[i].rect;
    int a = collider.x - this->camera.position.x;
    int b = collider.y - this->camera.position.y;
    int c = collider.w;
    int d = collider.h;

    SDL_Rect phy = {a, b, c, d};

    SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
    SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &phy);
    SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
*/  
  }

  for(Uint32 j = 0; j < CollisionSystem::Instance()->getColliders().size(); j++){
    SDL_Rect rect = CollisionSystem::Instance()->getColliders()[j].rect;

    int a = CollisionSystem::Instance()->getColliders()[j].rect.x - this->camera.position.x;
    int b = CollisionSystem::Instance()->getColliders()[j].rect.y - this->camera.position.y;
    int c = CollisionSystem::Instance()->getColliders()[j].rect.w;
    int d = CollisionSystem::Instance()->getColliders()[j].rect.h;

    SDL_Rect phy = {a, b, c, d};

    //remove
    SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
    SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &phy);
    SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
  }

  /*Shader*/
  //SDL_SetRenderDrawBlendMode(Renderer::Instance()->getRenderer(), SDL_BLENDMODE_BLEND);
  //SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 40, 125, 255*0.15);
  //SDL_RenderFillRect(Renderer::Instance()->getRenderer(), nullptr);
  //SDL_SetRenderDrawBlendMode(Renderer::Instance()->getRenderer(), SDL_BLENDMODE_NONE);
  /*Shader*/

  SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);


  /*
    int gh = (int)change;
    SDL_Rect viewport;
    SDL_RenderGetViewport(Renderer::Instance()->getRenderer(), &viewport);
    int x = viewport.x + viewport.w;
    int y = viewport.y;
    SDL_Rect rain{x-20,y+20,0,10};
    SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &rain);
    //SDL_RenderDrawRoundRect(SDL_Renderer* renderer, const SDL_Rect* rect, int radius)
  */

    //printf("{%d, %d}\n", viewport.x + viewport.w - 5,  viewport.y+5);

  for(Uint32 i = 0; i < this->bars.size(); i++){
    int r = bars[i].r;
    int g = bars[i].g;
    int b = bars[i].b;
    int a = bars[i].a;
    if(!bars[i].fill){
      SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), r, g, b, a);
      SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &bars[i].rect);
    }else{
      SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), r, g, b, a);
      SDL_RenderFillRect(Renderer::Instance()->getRenderer(), &bars[i].rect);
    }
  }
  
  SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);

}

std::vector<SpriteComponent>& SpriteSystem::getSprites(){
  return this->sprites;
} 


Aurora::ECS<SpriteComponent>* SpriteSystem::getSpriteComponentSystem(){
  return this->spriteComponentSystem;
}

std::unordered_map<Atlas, SDL_Texture*>& SpriteSystem::getTextures(){
  return this->textures;
}

SDL_Texture* SpriteSystem::getTexture(Atlas type){
  return this->textures[type];
}

void SpriteSystem::createSubscriptions(){


  eventManager->subscribe(EventType::INPUT, [this](const Event event){

    Uint32 entityIndex = spriteComponentSystem->getEntityIndex(event.entityA);

    SpriteComponent& sprite = this->sprites[entityIndex];

    sprite.src_rect.y = ((std::get<InputComponent>(event.data)).moveLeft) ? 2 * sprite.src_rect.h : sprite.src_rect.y;
    sprite.src_rect.y = ((std::get<InputComponent>(event.data)).moveRight) ? 3 * sprite.src_rect.h : sprite.src_rect.y;
    sprite.src_rect.y = ((std::get<InputComponent>(event.data)).moveUp) ? 0 : sprite.src_rect.y;
    sprite.src_rect.y = ((std::get<InputComponent>(event.data)).moveDown) ? sprite.src_rect.h : sprite.src_rect.y;

  });

  eventManager->subscribe(EventType::SPRITE_PHYSICS_UPDATE, [this](const Event event){

    Uint32 entityIndex = spriteComponentSystem->getEntityIndex(event.entityA);

    SpriteComponent& sprite = this->sprites[entityIndex];

    if(sprite.atl == RAIN)
      return;

    sprite.src_rect.y = ((std::get<Vec2>(event.data)).x < 0) ? 2 * sprite.src_rect.h : sprite.src_rect.y;
    sprite.src_rect.y = ((std::get<Vec2>(event.data)).x > 0) ? 3 * sprite.src_rect.h : sprite.src_rect.y;
    sprite.src_rect.y = ((std::get<Vec2>(event.data)).y < 0) ? 0 : sprite.src_rect.y;
    sprite.src_rect.y = ((std::get<Vec2>(event.data)).y > 0) ? sprite.src_rect.h : sprite.src_rect.y;

  });

  eventManager->subscribe(EventType::MOVEMENT, [this](const Event event){
    Uint32 entityIndex = spriteComponentSystem->getEntityIndex(event.entityA);

    this->sprites[entityIndex].dest_rect.x = std::get<Vec2>(event.data).x;
    this->sprites[entityIndex].dest_rect.y = std::get<Vec2>(event.data).y;
  });

  eventManager->subscribe(EventType::UPDATE_PLAYER_HEALTH, [this](const Event event){
    Uint32 max = std::get<HealthComponent>(event.data).max;
    Uint32 current = std::get<HealthComponent>(event.data).current;
    float healthPercentage = (static_cast<float>(current) / max);

    int barWidth = 100;
    int barHeight = 10;
    int barX = 10;
    int barY = 15;

    static float currentWidth = (float)static_cast<int>(barWidth * healthPercentage );

    /*
    Renderer is cleared after events are processed
    SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 216, 216, 216, 255);
    SDL_Rect backgroundRect = {barX-1, barY-1, barWidth+2, barHeight+2};
    SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &backgroundRect);

    SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 100, 255, 175, 255);
    SDL_Rect healthRect = {barX, barY, (int)currentWidth, barHeight};
    SDL_RenderFillRect(Renderer::Instance()->getRenderer(), &healthRect);
    */

    // Solution
    SDL_Rect backgroundRect = {barX-1, barY-1, barWidth+2, barHeight+2};
    this->bars.push_back(Bar{0, 0, 0, 0, backgroundRect});

    int r, g, b;
    int a = 255;

    // Change health bar color based on current health
    if(current == max){
      r = 100;
      g = 255;
      b = 175;
    } else if( current < max && healthPercentage > 0.25) {
      r = 255;
      g = 255;
      b = 175;
    } else{
      r = 255;
      g = 100;
      b = 100;
    }

    SDL_Rect healthRect = {barX, barY, (int)currentWidth, barHeight};
    this->bars.push_back(Bar{r, g, b, a, healthRect, true});

  });
  
}

Uint32 SpriteSystem::getInactiveIndex(){
  return inactiveIndex;
}

