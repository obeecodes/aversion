#include "../../include/components/colliderComponent.h"

ColliderComponent::ColliderComponent(SDL_Rect rect, Vec2 offset, ColliderType type)
    : rect(rect), offset(offset), type(type){}

ColliderComponent::ColliderComponent(SDL_Rect rect)
    : rect(rect), offset(Vec2{0, 0}), type(ColliderType::KINEMATIC) {}


ColliderComponent::ColliderComponent(SDL_Rect rect, ColliderType type)
    :rect(rect), offset(Vec2{0, 0}), type(type){}
