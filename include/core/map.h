#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include "../core/TMXParser.h"
#include <vector>
#include <unordered_map>

typedef struct Tile Tile;

struct Tile{
  Uint32 id; //tile id
  SDL_Rect source; //portion of texture
  SDL_Rect dimensions; //where to draw
  SDL_Rect collider; //collision box
  Tile(Uint32 id, SDL_Rect source, SDL_Rect dimensions, SDL_Rect collider) 
    : id(id), source(source), dimensions(dimensions), collider(collider){};
};

class Map{
public:

  static Map* Instance();
  void update();
  void initMap();
  Uint32 getMapWidth();
  Uint32 getMapHeight();
  std::vector<std::vector<std::vector<Tile>>>& getMap();
  SDL_Rect getTile(SDL_Rect location);
  SDL_Rect getWorldRegion();

private:
  static Map* instance;
  Map();
  std::vector<std::vector<std::vector<Tile>>> map;
  std::unordered_map<Uint32, std::vector<Object>> tileToCollider;
  Uint32 mapHeight;
  Uint32 mapWidth;
  SDL_Rect worldRegion;
  void assignCollider(Uint32 tileID, SDL_Rect source, SDL_Rect dimensions, Uint32 i, Uint32 j, Uint32 x, Uint32 y);

  void printMap();
  void printCollider();


};