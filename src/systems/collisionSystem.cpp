#include "../../include/systems/collisionSystem.h"
#include "../../include/systems/eventManager.h"
#include "../../include/components/vec2.h"

#include "../../include/entities/entityManager.h"
#include "../../include/core/renderer.h"

CollisionSystem* CollisionSystem::instance = nullptr;

CollisionSystem* CollisionSystem::Instance(){
  if(!instance){
    instance = new CollisionSystem();
  }
  return instance;
}

CollisionSystem::CollisionSystem()
: colliderComponentSystem(new Aurora::ECS<ColliderComponent>()),
  colliders(colliderComponentSystem->getComponents()),
  inactiveIndex(colliderComponentSystem->getInactiveIndex()),
  quadtree(new Quadtree(SDL_Rect{ 0, 0, 32*32, 32*32})),
  eventManager(EventManager::Instance()){

    //subscribe to events
    createSubscriptions();
}

void CollisionSystem::update(){

  //for(Uint32 i = 0; i < )
/*    int a = this->colliders[j].rect.x - CameraSystem::Instance()->getCamera().position.x;
    int b = this->colliders[j].rect.y - CameraSystem::Instance()->getCamera().position.y;
    int c = this->colliders[j].rect.w;
    int d = this->colliders[j].rect.h;

    SDL_Rect phy = {a, b, c, d};

      //remove
      SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
      SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &phy);
      SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);*/


      int pastX = this->colliders[0].rect.x;
      int pastY = this->colliders[0].rect.y;
      //this->physics[0].collider.y = this->transforms[0].position.y + 10;

  for(Uint32 i = 0; i < this->inactiveIndex; i++){
    for(Uint32 j = i + 1; j < this->inactiveIndex; j++){
      if(isCollision(this->colliders[i].rect, this->colliders[j].rect)){
        if(this->colliders[j].type == ColliderType::STATIC){
          SDL_Rect a = this->colliders[i].rect;
          SDL_Rect b = this->colliders[j].rect;

          //printf("Player: {%d, %d}\nWall: {%d, %d}\n", a.x, a.y, b.x, b.y);
                  //CollisionSystem::Instance()->getColliders()[0].rect.x = pastX;
                  //CollisionSystem::Instance()->getColliders()[0].rect.y = pastY;
            
          // Only used when colliding with walls
          //PhysicsSystem::Instance()->undoMove(getCollisionNormal(this->colliders[i].rect, this->colliders[j].rect));

          Uint32 entityA = colliderComponentSystem->getEntityID(i);

          Uint32 entityB = colliderComponentSystem->getEntityID(j);
          //std::cout << "Collision:\nEntity A: " << entityA << "\nEntity B: " << entityB << "\n";
          //eventManager->publish(Event{EventType::WALL_COLLISION, entityA, entityB, getCollisionNormal(this->colliders[i].rect, this->colliders[j].rect)});
        }
      }

      /*
       Before SpriteSystem::update is called, renderer is cleared, so this does not work as expected
        int a = this->colliders[j].rect.x - CameraSystem::Instance()->getCamera().position.x;
        int b = this->colliders[j].rect.y - CameraSystem::Instance()->getCamera().position.y;
        int c = this->colliders[j].rect.w;
        int d = this->colliders[j].rect.h;

        SDL_Rect phy = {a, b, c, d};

        //remove
        SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
        SDL_RenderDrawRect(Renderer::Instance()->getRenderer(), &phy);
        SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0, 0, 0, 255);
      */    
    }
  }
}

void CollisionSystem::updateCollider(int x, int y, int width, int height){

}

bool CollisionSystem::isCollision(SDL_Rect a, SDL_Rect b){
  return SDL_HasIntersection(&a, &b);
}

SDL_Rect CollisionSystem::getSweptRect(SDL_Rect& rect, int xVel, int yVel){
  SDL_Rect swept;
  swept.x = rect.x + xVel;
  swept.y = rect.y + yVel;
  swept.w = rect.w;
  swept.h = rect.h;

  if(xVel < 0){
    swept.x += xVel;
    swept.w -= xVel;
  }

  if(xVel > 0){
    swept.w += xVel;
  }

  if(yVel < 0){
    swept.y += yVel;
    swept.h -= yVel;
  }

  if(yVel > 0){
    swept.h += yVel;
  }

  return swept;
}

void CollisionSystem::createColliderComponent(ColliderComponent collider, Uint32 entityID){
  this->colliderComponentSystem->createComponent(collider, entityID);

  this->quadtree->insert(collider);
}

std::vector<ColliderComponent>& CollisionSystem::getColliders(){
  return this->colliders;
}

Vec2 CollisionSystem::getCollisionNormal(SDL_Rect& a, SDL_Rect& b){
  //let a be the moving object
  //let b be the motionless object
  SDL_Rect tempA = a;
  SDL_Rect tempB = b;

  int overlapLeft = (a.x + a.w) - b.x;
  int overlapRight = (b.x + b.w) - a.x;
  int overlapTop = (a.y + a.h) - b.y;
  int overlapBottom = (b.y + b.h) - a.y;

  int minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

 
  //fixes wall sliding sticking when sliding against adjacent collider
  if (minOverlap == overlapTop && minOverlap == overlapRight) {
  }

  if (minOverlap == overlapTop && minOverlap == overlapLeft) {
  }

  if (minOverlap == overlapBottom && minOverlap == overlapRight) {
  }

  if (minOverlap == overlapBottom && minOverlap == overlapLeft) {
  }



 if (minOverlap == overlapTop) {
      return Vec2{0, -1}; // Top side collision
  } else if (minOverlap == overlapBottom) {
     return Vec2{0, 1}; // Bottom side collision
  } else if (minOverlap == overlapLeft) {
      return Vec2{-1, 0}; // Left side collision
  } else if (minOverlap == overlapRight) {
      return Vec2{1, 0}; // Right side collision
  }

  return Vec2{0,0};
}



      //CollisionSystem::Instance()->getColliders()[0].rect.x = PhysicsSystem::Instance()->getTransforms()[0].position.x + CollisionSystem::Instance()->getColliders()[0].offset.x;
      //CollisionSystem::Instance()->getColliders()[0].rect.y = PhysicsSystem::Instance()->getTransforms()[0].position.y + CollisionSystem::Instance()->getColliders()[0].offset.y;

void CollisionSystem::createSubscriptions(){
  eventManager->subscribe(EventType::MOVEMENT, [this](const Event event){

    if(!colliderComponentSystem->hasComponent(event.entityA)){
      //entity does not have collider
      return;
    }

    Uint32 entityIndex = colliderComponentSystem->getEntityIndex(event.entityA);

    Vec2 newColliderPosition = std::get<Vec2>(event.data) + this->colliders[entityIndex].offset;

    this->colliders[entityIndex].rect.x = newColliderPosition.x;
    this->colliders[entityIndex].rect.y = newColliderPosition.y;

  });
  
  eventManager->subscribe(EventType::MOVE_INTENT, [this](const Event event){

    if(!colliderComponentSystem->hasComponent(event.entityA)){
      //entity does not have collider
      eventManager->publish(Event{EventType::MOVE_SUCCESS, event.entityA, std::get<Vec2>(event.data)});
      return;
    }

    Uint32 entityIndex = colliderComponentSystem->getEntityIndex(event.entityA);

    ColliderComponent collider = this->colliders[entityIndex];
    ColliderComponent colliderX = this->colliders[entityIndex];
    ColliderComponent colliderY = this->colliders[entityIndex];

    colliderX.rect.x += std::get<Vec2>(event.data).x;
    colliderY.rect.y += std::get<Vec2>(event.data).y;

    bool collision = false;
    bool collisionX;
    bool collisionY;

    for(Uint32 i = 0; i < this->inactiveIndex; i++){

      // Prevents object collision with self
      if(i == entityIndex){
        continue;
      }

      if(isCollision(colliderX.rect, this->colliders[i].rect)){
        collisionX = true;
        break;
      }
      
      collisionX = false;
    }

    for(Uint32 i = 0; i < this->inactiveIndex; i++){

      // Prevents object collision with self
      if(i == entityIndex){
        continue;
      }

      if(isCollision(colliderY.rect, this->colliders[i].rect)){
        collisionY = true;
        break;
      }
      
      collisionY = false;
    }

    if(collisionX && collisionY){
      collision = true;
    }

    Vec2 velocity{};
    if(!collisionX && !collisionY){
      velocity += std::get<Vec2>(event.data);
    } else if(collisionX && !collisionY){
      velocity.y += std::get<Vec2>(event.data).y;
    } else if(!collisionX && collisionY){
      velocity.x += std::get<Vec2>(event.data).x;
    }

    if(!collision){
      eventManager->publish(Event{EventType::MOVE_SUCCESS, event.entityA, velocity});
      //printf("Entity: %d\n", event.entityA);
    }

  });
}

