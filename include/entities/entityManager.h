#pragma once 

#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <vector>
#include <queue>
#include "entityArchetype.h"
#include "../core/jsonParser.h"
#include "../components/tag.h"
#include <set>

class EntityManager{
public:
  static EntityManager* Instance();
  void createRain();
  void createEntities();
  void createTileCollider(const SDL_Rect& rect);
  std::unordered_map<Uint32, std::set<TagType>>& getEntityToTag();
  std::unordered_map<TagType, std::vector<Uint32>>& getTagToEntities();

  //std::string 

  std::vector<Uint32> entityFlags;

private:
  std::queue<Uint32> reuseableID;
  std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, JSONValue>>> jsonMap;


  EntityManager();
  static EntityManager* instance;
  Uint32 entityID;

  //stores empty tag components for easy querying
  std::unordered_map<Uint32, std::set<TagType>> entityToTag;
  std::unordered_map<TagType, std::vector<Uint32>> tagToEntities;

  std::unordered_map<EntityType, std::function<void(Uint32)>>& entityArchetypes;
};