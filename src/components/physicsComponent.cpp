#include "../../include/components/physicsComponent.h"

PhysicsComponent::PhysicsComponent()
    : velocity(Vec2{}), acceleration(Vec2{}){}

PhysicsComponent::PhysicsComponent(Vec2 velocity, Vec2 acceleration)
    : velocity(velocity), acceleration(acceleration){}
