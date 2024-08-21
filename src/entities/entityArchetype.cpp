#include "../../include/entities/entityArchetype.h"
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
#include "../../include/core/tsuki.h"
#include "../../include/core/jsonParser.h"
#include "../../include/core/renderer.h"
#include "../../include/core/window.h"
#include <iostream>

namespace Archetype{

    Uint32 Player = PHYSICS | TRANSFORM | COLLIDER | CAMERA | SPRITE | INPUT | AUDIO | STATE | HEALTH;
    Uint32 Pup = PHYSICS | TRANSFORM | COLLIDER | SPRITE | HEALTH | AI;
    Uint32 Enemy = AI | PHYSICS | TRANSFORM | COLLIDER | SPRITE | AUDIO | STATE | HEALTH;
    Uint32 Particle = PHYSICS | TRANSFORM | SPRITE;
    Uint32 Rain = PHYSICS | TRANSFORM | SPRITE ;

    Uint32 getComponentFlags(const std::string& entity){
        if(entity == "Player"){
            return Player;
        }else if(entity == "Pup"){
            return Pup;
        }else if(entity == "Enemy"){
            return Enemy;
        }else if(entity == "Particle"){
            return Particle;
        }else if(entity == "Rain"){
            return Rain;
        }
        return (Uint32)0;
    }
}

EntityArchetype* EntityArchetype::instance = nullptr;

EntityArchetype* EntityArchetype::Instance(){
    if(!instance){
        instance = new EntityArchetype();
    }

    return instance;
}

EntityArchetype::EntityArchetype(){}


void EntityArchetype::initArchetypes(){
    entityArchetypes[EntityType::Player] = {
        [](Uint32 entityID) {

            // Position
            int x{644};
            int y{644};

            // Health Component
            Uint32 health{200};

            // Physics Component
            Vec2 velocity{};
            Vec2 acceleration{};

            // Transform Component
            Vec2 position{x, y};
            int rotation{0};
            Vec2 scale{1, 1};

            // Collider Component
            SDL_Rect collider{x, y, 8, 18};
            Vec2 offset{12, 10};

            // Sprite Component
            std::string texture{"Player"};
            SDL_Rect source{0, 32, 32, 32};
            SDL_Rect destination{x, y, 32, 32};

            // Audio Component
            std::string audio{"LIGHTNING"};


            HealthSystem::Instance()->createHealthComponent(HealthComponent{health}, entityID);

            PhysicsSystem::Instance()->createPhysicsComponent(PhysicsComponent{velocity, acceleration}, entityID);
            PhysicsSystem::Instance()->createTransformComponent(TransformComponent{position, rotation, scale}, entityID);
            
            CollisionSystem::Instance()->createColliderComponent(ColliderComponent{collider, offset}, entityID);

            SpriteSystem::Instance()->createSpriteComponent(SpriteComponent{PLAYER, source, destination}, entityID);

            AudioSystem::Instance()->createAudioComponent(audio, entityID);

            InputSystem::Instance()->createInputComponent(InputComponent{}, entityID);   
            
            CameraSystem::Instance()->createCameraComponent(CameraComponent{}, entityID);

            StateSystem::Instance()->createStateComponent(StateComponent(), entityID);
      
        }

    };

    entityArchetypes[EntityType::Pup] = {
        [](Uint32 entityID) {

            // Position
            int x{64}; //670 
            int y{64}; //670

            // Health Component
            Uint32 health{50};

            // Physics Component
            Vec2 velocity{};
            Vec2 acceleration{};

            // Transform Component
            Vec2 position{x, y};
            int rotation{0};
            Vec2 scale{1, 1};

            // Collider Component
            SDL_Rect collider{x, y, 8, 14};
            Vec2 offset{4, 2};

            // Sprite Component
            std::string texture{"Pup"};
            SDL_Rect source{0, 16, 16, 16};
            SDL_Rect destination{x, y, 16, 16};
            
            // AI Component
            std::string script{"src/scripts/pup.lua"};


            HealthSystem::Instance()->createHealthComponent(HealthComponent{health}, entityID);

            PhysicsSystem::Instance()->createPhysicsComponent(PhysicsComponent{velocity, acceleration}, entityID);
            PhysicsSystem::Instance()->createTransformComponent(TransformComponent{position, rotation, scale}, entityID);
            
            CollisionSystem::Instance()->createColliderComponent(ColliderComponent{collider, offset}, entityID);

            SpriteSystem::Instance()->createSpriteComponent(SpriteComponent{PUP, source, destination}, entityID);

            AISystem::Instance()->createAIComponent(AIComponent{script}, entityID);
            Tsuki::Instance()->setPupPosition(position);
            
        }

    };

    entityArchetypes[EntityType::Rain] = {
        [](Uint32 entityID) {    

            // Position
            int x;
            int y;

            switch(rand() % 2){
                case 0:
                    x = rand() % (1280 - 32);
                    y = 32;

                break;

                case 1:
                    x = 1280 - 32;
                    y = rand() % (1280 - 32);
                break;
            }

            // Physics Component
            int mag = rand() % 5 + 5; // 5 - 9
            Vec2 velocity{-3, 7};
            Vec2 acceleration{};
            velocity.normalize();
            velocity*=mag;

            // Transform Component
            Vec2 position{x, y};
            int rotation{0};
            Vec2 scale{1, 1};

            // Sprite Component
            std::string texture{"Rain"};
            SDL_Rect source{0, 0, 8, 8};
            SDL_Rect destination{x, y, 16, 16};

            PhysicsSystem::Instance()->createPhysicsComponent(PhysicsComponent{velocity, acceleration}, entityID);
            PhysicsSystem::Instance()->createTransformComponent(TransformComponent{position, rotation, scale}, entityID);

            SpriteSystem::Instance()->createSpriteComponent(SpriteComponent{RAIN, source, destination}, entityID);
            
        }

    };
/*
    entityArchetypes[EntityType::Enemy] = {
        [](Uint32 entityID) {
            
        }

    };

    entityArchetypes[EntityType::NPC] = {
        [](Uint32 entityID) {
            
        }

    };

    entityArchetypes[EntityType::NONE] = {
        [](Uint32 entityID) {
            
        }

    };
*/
}

std::unordered_map<EntityType, std::function<void(Uint32)>>& EntityArchetype::getEntityArchetypes(){
    return entityArchetypes;
}
