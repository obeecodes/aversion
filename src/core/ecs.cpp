#include "../../include/core/ecs.h"
#include "../../include/components/aiComponent.h"
#include "../../include/components/audioComponent.h"
#include "../../include/components/cameraComponent.h"
#include "../../include/components/colliderComponent.h"
#include "../../include/components/dialogueComponent.h"
#include "../../include/components/healthComponent.h"
#include "../../include/components/inputComponent.h"
#include "../../include/components/lightComponent.h"
#include "../../include/components/physicsComponent.h"
#include "../../include/components/renderComponent.h"
#include "../../include/components/stateComponent.h"
#include "../../include/components/spriteComponent.h"
#include "../../include/components/transformComponent.h"
#include <iostream>
#include "assert.h"

template <typename T>
Aurora::ECS<T>::ECS(){
    this->inactiveIndex = 0;
    
    this->components = std::vector<T>(256);
    this->denseComponents = std::vector<Uint32>(256);
    this->sparseComponents = std::vector<Uint32>(256);
}

// if I need to, I could keep track of inactive components in another data structure

template <typename T>
void Aurora::ECS<T>::createComponent(T component, Uint32 entityID){
    std::cout << "Entity: " << entityID << std::endl;
    this->components[this->inactiveIndex] = component; //adds new component to vector
    this->denseComponents[this->inactiveIndex] = entityID; //adds new entity to denseComponents

    this->sparseComponents[entityID] = this->inactiveIndex; // sets index of entity in sparseComponents

    //adds entity to set for quick lookup
    this->active.insert(entityID);
    this->inactiveIndex++;

    // resize dense
    if(this->inactiveIndex == denseComponents.size() - 1){
        //std::cout << "Type of class: " << typeid(*this).name() << std::endl;
        //std::cout << "Resizing dense!\nCurrent Size: " << denseComponents.size() << std::endl;

        this->components.resize(this->components.size() * 2);
        this->denseComponents.resize(this->denseComponents.size() * 2);
        //std::cout << "New Size: " << this->denseComponents.size() << "\n" << std::endl;
    }

    // sets active status of entity
    // marks entity as having component
    //this->active.set(entityID); 

    // resize sparse 
    if(entityID == sparseComponents.size() - 1){
        //std::cout << "Resizing sparse!\nCurrent Size: " << sparseComponents.size() << std::endl;
        this->sparseComponents.resize(this->sparseComponents.size() * 2);
        //std::cout << "New Size: " << sparseComponents.size() << "\n" << std::endl;
    }

}

template <typename T>
void Aurora::ECS<T>::destroyComponent(Uint32 entityID){

    // if entity does not have an associated component, return
    /*if(!this->active.test(entityID)){
        return;
    }*/

    // swap entity component to last position in vector
    std::swap(this->components[this->sparseComponents[entityID]], this->components[this->inactiveIndex - 1]);
    std::swap(this->denseComponents[this->sparseComponents[entityID]], this->denseComponents[this->inactiveIndex - 1]);
    std::swap(this->sparseComponents[entityID], this->sparseComponents[this->denseComponents[this->inactiveIndex - 1]]); // updates entity indexes

    // sets inactive status to entity
    //this->active.reset(entityID);

    this->inactiveIndex--;

}

template <typename T>
std::vector<T>& Aurora::ECS<T>::getComponents(){

    return this->components;
    
}

template <typename T>
Uint32& Aurora::ECS<T>::getInactiveIndex(){

    return this->inactiveIndex;
}

template <typename T>
T Aurora::ECS<T>::getEntityComponent(Uint32 entityID){
    return this->components[this->sparseComponents[entityID]];
    
}

template <typename T>
Uint32 Aurora::ECS<T>::getEntityIndex(Uint32 entityID){

    if(entityID != 0 && denseComponents[sparseComponents[entityID]] == 0 ){
        return SDL_MAX_UINT32;
    }
    assert(entityID == denseComponents[sparseComponents[entityID]]);
    return this->sparseComponents[entityID];
}

// given that I created a vector to ensure that the invariant is true
// sparse[ dense[ i ] ] = i 
// I can use this to get the corresponding entityID from any system update loop
template <typename T>
Uint32 Aurora::ECS<T>::getEntityID(Uint32 index){
    if(index != 0 && sparseComponents[denseComponents[index]] == 0 ){
        //return SDL_MAX_UINT32;
    }
    assert(index == sparseComponents[denseComponents[index]]);
    return this->denseComponents[index];
}

template <typename T>
bool Aurora::ECS<T>::hasComponent(Uint32 entityID){
    return this->active.find(entityID) != this->active.end();
}

//use vectors instead of unordered maps
/*
    private:
        std::unordered_map<Uint32, Uint32> entityToIndex;
        std::unordered_map<Uint32, Uint32> indexToEntity;
template <typename T>
void Aurora::ECS<T>::createComponent(T component, Uint32 entityID){

    this->components.push_back(component); //adds new component to end of vector
    std::swap(this->components[this->inactiveIndex], this->components.back()); //swaps first inactive component with new component

    if(!(this->indexToEntity.find(entityID) == this->indexToEntity.end())){//Checks if entity is in map
        this->entityToIndex[this->indexToEntity[this->inactiveIndex]] = this->components.size() - 1; //updates index of first inactive component to end of vector
        this->indexToEntity[this->components.size() - 1] = this->entityToIndex[this->indexToEntity[this->inactiveIndex]]; //sets last index to first inactive component
    }else{
        this->entityToIndex[entityID] = this->inactiveIndex; //adds entity to unordered map
        this->indexToEntity[this->inactiveIndex] = entityID; //updates index to map to entity
    }
    this->inactiveIndex++; //updates inactive index
}

template <typename T>
void Aurora::ECS<T>::destroyComponent(Uint32 entityID){
    std::swap(this->components[this->entityToIndex[entityID]], this->components[this->inactiveIndex - 1]); //swaps inactive component with last active component
    this->entityToIndex[this->indexToEntity[this->inactiveIndex - 1]] = this->entityToIndex[entityID]; //updates index of last active component
    this->indexToEntity[this->entityToIndex[entityID]] = this->indexToEntity[this->inactiveIndex - 1]; //updates index mapping

    this->entityToIndex[entityID] = this->inactiveIndex - 1; //updates entity index
    this->indexToEntity[this->inactiveIndex - 1] = entityID; //updates index mapping

    this->inactiveIndex--; // updates inactive index
}
*/

template class Aurora::ECS<AIComponent>;
template class Aurora::ECS<AudioComponent>;
template class Aurora::ECS<CameraComponent>;
template class Aurora::ECS<ColliderComponent>;
template class Aurora::ECS<DialogueComponent>;
template class Aurora::ECS<HealthComponent>;
template class Aurora::ECS<InputComponent>;
template class Aurora::ECS<LightComponent>;
template class Aurora::ECS<PhysicsComponent>;
template class Aurora::ECS<RenderComponent>;
template class Aurora::ECS<StateComponent>;
template class Aurora::ECS<SpriteComponent>;
template class Aurora::ECS<TransformComponent>;
    
