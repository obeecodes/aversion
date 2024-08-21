#include "../../include/systems/physicsSystem.h"
#include "../../include/systems/collisionSystem.h"

#include <iostream>

PhysicsSystem* PhysicsSystem::instance = nullptr;

PhysicsSystem* PhysicsSystem::Instance(){
  if(!instance){
    instance = new PhysicsSystem();
  }
  return instance;
}

PhysicsSystem::PhysicsSystem()
: physicsComponentSystem(new Aurora::ECS<PhysicsComponent>()),
  physics(physicsComponentSystem->getComponents()),
  inactiveIndex(physicsComponentSystem->getInactiveIndex()),
  transformComponentSystem(new Aurora::ECS<TransformComponent>()),
  transforms(transformComponentSystem->getComponents()),
  eventManager(EventManager::Instance()){
    createSubscriptions();
    
}

void PhysicsSystem::update(){
  // an oberserver should handle this


  // create an event listener for physics updates

  for(Uint32 i = 0; i < this->inactiveIndex; i++){
    //this->physics[i].velocity += this->physics[i].acceleration;

    //if moving object would cause a collision, then don't move it
    

    // assume that every entity with a physics component has a transform component associated with it
    
    
    //this->transforms[i].position += this->physics[i].velocity;
    //Uint32 entity = physicsComponentSystem->getEntityID(i);
    //eventManager->publish(Event{EventType::MOVEMENT, entity, this->transforms[i].position});

    //Let's try something new, an intent to move
    this->physics[i].velocity += this->physics[i].acceleration;

    Uint32 entity = physicsComponentSystem->getEntityID(i);

    //Not every entity that has a physics component has a collider
    eventManager->publish(Event{EventType::MOVE_INTENT, entity, this->physics[i].velocity});

  }
}

void PhysicsSystem::undoMove(Vec2 v){

  Vec2 oldVec = this->physics[0].velocity;
  int dx = this->physics[0].velocity.x; 
  int dy = this->physics[0].velocity.y; 


  //testing player movement
  // 
  
  if(dx){
    this->transforms[0].position.x -= this->physics[0].velocity.x;
  }
  
  if(dy){
    this->transforms[0].position.y -= this->physics[0].velocity.y;
  }

  Vec2 newVec = oldVec - v * oldVec.dot(v);

  this->physics[0].velocity = newVec;
  this->transforms[0].position += newVec;

  CollisionSystem::Instance()->getColliders()[0].rect.x = PhysicsSystem::Instance()->getTransforms()[0].position.x + CollisionSystem::Instance()->getColliders()[0].offset.x;
  CollisionSystem::Instance()->getColliders()[0].rect.y = PhysicsSystem::Instance()->getTransforms()[0].position.y + CollisionSystem::Instance()->getColliders()[0].offset.y;
  
}


void PhysicsSystem::createPhysicsComponent(PhysicsComponent physicsComponent, Uint32 entityID){
  this->physicsComponentSystem->createComponent(physicsComponent, entityID);
}

void PhysicsSystem::createTransformComponent(TransformComponent transformcomponent, Uint32 entityID){
  this->transformComponentSystem->createComponent(transformcomponent, entityID);
}

std::vector<TransformComponent>& PhysicsSystem::getTransforms(){
  return this->transforms;
}

std::vector<PhysicsComponent>& PhysicsSystem::getPhysics(){
  return this->physics;
}

Aurora::ECS<PhysicsComponent>* PhysicsSystem::getPhysicsComponentSystem(){
  return this->physicsComponentSystem;
}

Aurora::ECS<TransformComponent>* PhysicsSystem::getTransformComponentSystem(){
  return this->transformComponentSystem;
}

void PhysicsSystem::createSubscriptions(){
  eventManager->subscribe(EventType::WALL_COLLISION, [this](const Event event){


    if(!physicsComponentSystem->hasComponent(event.entityA) || !physicsComponentSystem->hasComponent(event.entityB)){
      //entity does not have collider
      return;
    }
    Uint32 entityAIndex = physicsComponentSystem->getEntityIndex(event.entityA);
    Uint32 entityBIndex = physicsComponentSystem->getEntityIndex(event.entityB);

    Vec2 oldVec = this->physics[entityAIndex].velocity;
    int dx = oldVec.x; 
    int dy = oldVec.y;

    if(dx){
      this->transforms[entityAIndex].position.x -= oldVec.x;
    }
    if(dy){
      this->transforms[entityAIndex].position.y -= oldVec.y;
    }

    Vec2 v = std::get<Vec2>(event.data);

    Vec2 newVec = oldVec - v * oldVec.dot(v);

    this->physics[entityAIndex].velocity = newVec;
    this->transforms[entityAIndex].position += newVec;


    //CollisionSystem::Instance()->getColliders()[0].rect.x = PhysicsSystem::Instance()->getTransforms()[0].position.x + CollisionSystem::Instance()->getColliders()[0].offset.x;
    //CollisionSystem::Instance()->getColliders()[0].rect.y = PhysicsSystem::Instance()->getTransforms()[0].position.y + CollisionSystem::Instance()->getColliders()[0].offset.y;

    Event newEvent{EventType::MOVEMENT, event.entityA, event.entityB, Vec2{this->transforms[entityAIndex].position}};

    eventManager->publish(newEvent);
  });

  eventManager->subscribe(EventType::INPUT, [this](const Event event){

    Uint32 entityIndex = physicsComponentSystem->getEntityIndex(event.entityA);

    int dx = 0;
    int dy = 0;

    dx += (std::get<InputComponent>(event.data).moveLeft ? - 1 : 0);
    dx += (std::get<InputComponent>(event.data).moveRight ? 1 : 0);
    dy += (std::get<InputComponent>(event.data).moveUp ? - 1 : 0);
    dy += (std::get<InputComponent>(event.data).moveDown ? 1 : 0);

  
    this->physics[entityIndex].velocity.x = dx*2;
    this->physics[entityIndex].velocity.y = dy*2;
    
  });

  eventManager->subscribe(EventType::MOVE_SUCCESS, [this](const Event event){

    Uint32 entityIndex = physicsComponentSystem->getEntityIndex(event.entityA);
    int x;
    int y;

    this->transforms[entityIndex].position += std::get<Vec2>(event.data);

    if(this->transforms[entityIndex].position.x < 5 || this->transforms[entityIndex].position.y > 1248){

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
      this->transforms[entityIndex].position.x = x;
      this->transforms[entityIndex].position.y = y;
    }

    eventManager->publish(Event{EventType::MOVEMENT, event.entityA, Vec2{this->transforms[entityIndex].position}});
    eventManager->publish(Event{EventType::SPRITE_PHYSICS_UPDATE, event.entityA, std::get<Vec2>(event.data)});
    if(event.entityA == 0){
      eventManager->publish(Event{EventType::CAMERA_UPDATE, event.entityA, this->transforms[entityIndex].position});
      eventManager->publish(Event{EventType::PLAYER_MOVEMENT, event.entityA, this->transforms[entityIndex].position});
    }
  });

  eventManager->subscribe(EventType::UPDATE_VELOCITY, [this](const Event event){

    Uint32 entityIndex = physicsComponentSystem->getEntityIndex(event.entityA);

    float dx = 0;
    float dy = 0;

    dx += std::get<Vec2>(event.data).x;
    dy += std::get<Vec2>(event.data).y;

    Vec2 vel{dx, dy};
    vel.normalize();



  
    this->physics[entityIndex].velocity = vel * 2;
    
  });


}

Uint32 PhysicsSystem::getInactiveIndex(){
  return inactiveIndex;
}
