#include "../../include/entities/entityManager.h"
#include "../../include/components/spriteComponent.h"
#include "../../include/components/colliderComponent.h"
#include "../../include/components/audioComponent.h"
#include "../../include/systems/spriteSystem.h"
#include "../../include/systems/stateSystem.h"
#include "../../include/systems/collisionSystem.h"
#include "../../include/systems/physicsSystem.h"
#include "../../include/systems/audioSystem.h"
#include "../../include/systems/inputSystem.h"
#include "../../include/systems/cameraSystem.h"
#include "../../include/systems/aiSystem.h"
#include "../../include/systems/healthSystem.h"
#include "../../include/core/jsonParser.h"
#include "../../include/core/renderer.h"
#include "../../include/core/window.h"
#include <cstdlib>
#include <iostream>

#include <set>
#include "../../include/core/jsonParser.h"
#include "../../include/entities/entityArchetype.h"
#include "../../include/components/vec2.h"

const int TILE_SIZE = 32;

EntityManager* EntityManager::instance = nullptr;

EntityManager::EntityManager()
: entityArchetypes(EntityArchetype::Instance()->getEntityArchetypes()){
    this->entityID = 0;
    this->jsonMap = JSONParser::Instance()->getJSONMap();
    EntityArchetype::Instance()->initArchetypes();
    
}

EntityManager* EntityManager::Instance(){
    if(!instance){

    srand((unsigned)time(NULL));
        instance = new EntityManager();
    }
    return instance;
}

void EntityManager::createEntities() {

    EntityArchetype::Instance()->getEntityArchetypes()[EntityType::Player](entityID++);
    EntityArchetype::Instance()->getEntityArchetypes()[EntityType::Pup](entityID++);

    //for(Uint32 i = 0; i < 200 ; i++){
      //  EntityArchetype::Instance()->getEntityArchetypes()[EntityType::Rain](entityID++);
    //}

    //clear JSONParser memory after using it
    
}

void EntityManager::createTileCollider(const SDL_Rect& rect){

    CollisionSystem::Instance()->createColliderComponent(ColliderComponent{rect, ColliderType::STATIC}, this->entityID);
    entityToTag[this->entityID].insert(TagType::TILE);
    tagToEntities[TagType::TILE].push_back(this->entityID);
    this->entityID++;
}

void EntityManager::createRain(){
    entityArchetypes[EntityType::Rain](entityID++);
}



std::unordered_map<Uint32, std::set<TagType>>& EntityManager::getEntityToTag(){
    return entityToTag;
}

std::unordered_map<TagType, std::vector<Uint32>>& EntityManager::getTagToEntities(){
    return tagToEntities;
}


