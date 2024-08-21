#include "../../include/core/map.h"
#include "../../include/systems/renderSystem.h"
#include "../../include/core/renderer.h"
#include "../../include/systems/cameraSystem.h"
#include "../../include/systems/collisionSystem.h"
#include "../../include/systems/spriteSystem.h"
#include "../../include/entities/entityManager.h"
#include "../../include/components/spriteComponent.h"
#include "../../include/components/colliderComponent.h"
#include <SDL2/SDL.h>

const int TILE_SIZE = 32;

Map* Map::instance = nullptr;

auto isTile = [](const std::string& gid) -> bool{
  try{
    std::stoi(gid);
  } catch(const std::exception& e){
    return false;
  }
  return true;
};

void Map::initMap(){

  std::vector<std::vector<std::vector<Uint32>>>& temp_map = TMXParser::Instance()->getMap();
  this->map.resize(temp_map.size());
  
  for(Uint32 i = 0; i < temp_map.size(); i++){
    this->map[i].resize(temp_map[i].size());
  }

  Uint32 map_width = TMXParser::Instance()->getMapWidth();
  Uint32 map_height = TMXParser::Instance()->getMapHeight();

  this->mapWidth = map_width;
  this->mapHeight = map_height;

  this->worldRegion = SDL_Rect{0,0, static_cast<int>(map_width) * TILE_SIZE, static_cast<int>(map_height) * TILE_SIZE};

  Uint32 tile_width = TMXParser::Instance()->getTileWidth();
  Uint32 tile_height = TMXParser::Instance()->getTileHeight();

  for(Uint32 i = 0; i < temp_map.size(); i++){
    for(Uint32 j = 0; j < temp_map[i].size(); j++){
      for(Uint32 k = 0; k < temp_map[i][j].size(); k++){
        Uint32 id = temp_map[i][j][k];

        SDL_Rect source;
        source.x = (id - 1) % tile_width;
        source.x *= tile_width;

        source.y = (id - 1) / tile_height;
        source.y *= tile_height;

        source.w = tile_width;
        source.h = tile_height;

        SDL_Rect dimensions;
        dimensions.x = k * TILE_SIZE;
        dimensions.y = j * TILE_SIZE;
        dimensions.w = TILE_SIZE;
        dimensions.h = TILE_SIZE;
        //printf("{%d, %d, %d, %d}\n", dimensions.x, dimensions.y, dimensions.w, dimensions.h);
        //assignCollider(id, source, dimensions, i, j, k * tile_width, (j + 1) * tile_height);

        this->map[i][j].emplace_back(id, source, dimensions, SDL_Rect{0,0,0,0});

      }
    }
  }
  
  //vector of colliders
  std::vector<Object>& temp_objects = TMXParser::Instance()->getObjects();
    //assigns colliders
  for(Uint32 i = 0; i < temp_objects.size(); i++){
    SDL_Rect rect = {
      static_cast<int>(temp_objects[i].x),
      static_cast<int>(temp_objects[i].y),
      static_cast<int>(temp_objects[i].width),
      static_cast<int>(temp_objects[i].height)
    };

    /*
    printf("{%d, %d, %d, %d}\n", rect.x, rect.y, rect.w, rect.h);
    Uint32 row = temp_objects[i].y >> 5;
    Uint32 col = temp_objects[i].x >> 5;
    //printf("{x: %d, y: %d}\n", row, col);
    //this->map[0][row][col].collider = rect;

    if(rect.x == 208 && rect.y == 128){
      printf("Strange!\n");
    }
    */
    EntityManager::Instance()->createTileCollider(rect);
    /*
    if(CollisionSystem::Instance()->getColliders().back().rect.x == 208 
    && CollisionSystem::Instance()->getColliders().back().rect.y == 128){ //row == 6 && col == 4)
    printf("Bloop!\n");
    }else{
      SDL_Rect r = CollisionSystem::Instance()->getColliders().back().rect;
      printf("{%d, %d, %d, %d}\n", r.x, r.y, r.w, r.h);
    }
    */
  }

  TMXParser::Instance()->freeTMXParser();
}

Map* Map::Instance(){
  if(!instance){
    instance = new Map();
  }
  return instance;
}

Map::Map(){}

void Map::update(){

  CameraComponent camera = CameraSystem::Instance()->getCamera();

  // renders map
  SDL_Rect mapRect;
  for(Uint32 i = 0; i < this->mapHeight; i++){
    for(Uint32 j = 0; j < this->mapWidth; j++){
      mapRect.x = (this->map[0][i][j].dimensions.x - camera.position.x);
      mapRect.y = (this->map[0][i][j].dimensions.y - camera.position.y);
      mapRect.w = TILE_SIZE;
      mapRect.h = TILE_SIZE;
      //mapRect.w = this->camera.width;
      //mapRect.h = this->camera.height;
      SDL_RenderCopy(Renderer::Instance()->getRenderer(), SpriteSystem::Instance()->getTextures()[WORLD], &(this->map[0][i][j].source), &mapRect);
    /*
    int a = this->map[0][i][j].collider.x - camera.position.x;
    int b = this->map[0][i][j].collider.y - camera.position.y;
    int c = this->map[0][i][j].collider.w;
    int d = this->map[0][i][j].collider.h;

    SDL_Rect phy = {a, b, c, d};

      //remove
      SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
      SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &phy);
      SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
    */

    }
  }
}


//We probably don't need this
/*
void Map::assignCollider(Uint32 tileID, SDL_Rect source, SDL_Rect dimensions, Uint32 i, Uint32 j, Uint32 x, Uint32 y){
  
  SDL_Rect collider = {0,0,0,0};

  for(Uint32 l = 0; l < this->tileToCollider[tileID].size(); l++){
    if(x == this->tileToCollider[tileID][l].x && y == this->tileToCollider[tileID][l].y){
      collider.x = this->tileToCollider[tileID][l].x;
      collider.y = this->tileToCollider[tileID][l].y - TILE_SIZE;
      collider.w = TILE_SIZE;
      collider.h = TILE_SIZE;
    }
  }

  //ColliderComponent{collider};
  //CollisionSystem::Instance()->createColliderComponent();
  this->map[i][j].emplace_back(tileID, source, dimensions, collider);
  


}
*/

SDL_Rect Map::getTile(SDL_Rect location){
  int i = location.y >> 5; ///32;
  int j = location.x >> 5; ///32;
  SDL_Rect collider = this->map[0][i][j].collider;

  return collider;
  //return SDL_Rect{182/32*32, 562/32*32, 32, 32};

}

Uint32 Map::getMapWidth(){
  return this->mapWidth;
}

Uint32 Map::getMapHeight(){
  return this->mapHeight;
}

std::vector<std::vector<std::vector<Tile>>>& Map::getMap(){
  return this->map;
}

void Map::printMap(){
  for (Uint32 i = 0; i < map.size(); i++) {
    for (Uint32 j = 0; j < map[i].size(); j++) {
      for (Uint32 k = 0; k < map[i][j].size(); k++) {
        std::cout << map[i][j][k].id << " ";
      }
      std::cout << "\n";
    }
  }
}

void Map::printCollider(){
  for (Uint32 i = 0; i < map.size(); i++) {
    for (Uint32 j = 0; j < map[i].size(); j++) {
        printf("%2d: ", j);
      for (Uint32 k = 0; k < map[i][j].size(); k++) {
        printf("%3d ", map[i][j][k].collider.y/32);
      }
      std::cout << "\n";
    }
  }
}

SDL_Rect Map::getWorldRegion(){
  return this->worldRegion;
}