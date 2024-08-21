#pragma once

#include <SDL2/SDL_stdinc.h>
#include <string>
#include <functional>
#include <unordered_map>

enum Component{
  AI = 1 << 0,
  AUDIO = 1 << 1,
  CAMERA = 1 << 2,
  COLLIDER = 1 << 3,
  DIALOGUE = 1 << 4,
  HEALTH = 1 << 5,
  INPUT = 1 << 6,
  LIGHT = 1 << 7,
  PARTICLE = 1 << 8,
  PHYSICS = 1 << 9,
  SPRITE = 1 << 10,
  TRANSFORM = 1 << 11,
  STATE = 1 << 12,
  // = 1 << 13,
  // = 1 << 14,
  // = 1 << 15,
};

enum class EntityType{
  Player,
  Pup,
  Rain,
  Enemy,
  NPC,
  NONE,
};

class EntityArchetype{
public: 
    static EntityArchetype* Instance();
    void initArchetypes();
    std::unordered_map<EntityType, std::function<void(Uint32)>>& getEntityArchetypes();

private:
    static EntityArchetype* instance;
    EntityArchetype();
    std::unordered_map<EntityType, std::function<void(Uint32)>> entityArchetypes;

};




/*
{

    //
    //this->entityFlags.push_back(ALIVE | AUDIO | CAMERA | HEALTH | TRANSFORM | PHYSICS | RENDER | SPRITE );

    if(componentFlags & AI){
        std::string script = std::get<std::string>(jsonMap[entity]["AI"]["script"]);
        AISystem::Instance()->createAIComponent(AIComponent{script}, this->entityID);

    }
    
    if(componentFlags & AUDIO){
        AudioSystem::Instance()->createAudioComponent(std::get<std::string>(jsonMap[entity]["Audio"]["sample"]), this->entityID);
    }
    
    if(componentFlags & CAMERA){
        //use default camera
        CameraSystem::Instance()->createCameraComponent(CameraComponent{}, this->entityID);
        //CameraSystem::Instance()->createCameraComponent(CameraComponent{Vec2{0,0}, 64, 64}, this->entityID);
    }
    
    if(componentFlags & COLLIDER){

        std::vector<float> rectVec = std::get<std::vector<float>>(jsonMap[entity]["Collider"]["rect"]);

        SDL_Rect rect{
            (int)rectVec[0],
            (int)rectVec[1],
            (int)rectVec[2],
            (int)rectVec[3]
        };

       Vec2 vec = std::get<std::vector<float>>(jsonMap[entity]["Collider"]["offset"]);

        CollisionSystem::Instance()->createColliderComponent(ColliderComponent{rect, vec}, this->entityID);
    }
    
    if(componentFlags & DIALOGUE){
        
    }
    
    if(componentFlags & HEALTH){
        HealthSystem::Instance()->createHealthComponent(HealthComponent{std::get<Uint32>(jsonMap[entity]["Health"]["max"])}, this->entityID);
    }


    
    if(componentFlags & STATE){
        StateSystem::Instance()->createStateComponent(StateComponent(), this->entityID);
    }
    
    if(componentFlags & INPUT){
        InputSystem::Instance()->createInputComponent(InputComponent{}, this->entityID);    
    }
    
    if(componentFlags & LIGHT){
        
    }
    
    if(componentFlags & PARTICLE){
        
    }
    
    if(componentFlags & PHYSICS){
        Vec2 vel = std::get<std::vector<float>>(jsonMap[entity]["Physics"]["velocity"]);
        Vec2 accel = std::get<std::vector<float>>(jsonMap[entity]["Physics"]["acceleration"]);
        //printf("{%.2f, %.2f}\n", vel.x, vel.y);
        PhysicsComponent physicscomp{vel, accel};
        TransformComponent transformcomp{Vec2{std::get<std::vector<float>>(jsonMap[entity]["Transform"]["position"])}};
        PhysicsSystem::Instance()->createPhysicsComponent(physicscomp, this->entityID);
        PhysicsSystem::Instance()->createTransformComponent(transformcomp, this->entityID);
        
    }
    
    if(componentFlags & SPRITE){

        std::vector<float> srcVec = std::get<std::vector<float>>(jsonMap[entity]["Sprite"]["source"]);

        SDL_Rect src{
            (int)srcVec[0],
            (int)srcVec[1],
            (int)srcVec[2],
            (int)srcVec[3]
        };

        std::vector<float> destVec = std::get<std::vector<float>>(jsonMap[entity]["Sprite"]["destination"]);

        SDL_Rect dest{
            (int)destVec[0],
            (int)destVec[1],
            (int)destVec[2],
            (int)destVec[3]
        };

        std::string texture = std::get<std::string>(jsonMap[entity]["Sprite"]["texture"]);

        SpriteSystem::Instance()->createSpriteComponent(texture, src, dest, this->entityID);
        
    }
    
    if(componentFlags & TRANSFORM){
        // assume that every entity with a physics component has a transform component associated with it
    }

}*/