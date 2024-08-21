#include "../../include/components/transformComponent.h"

TransformComponent::TransformComponent()
    :   position(Vec2{}), rotation(0), scale(Vec2{1, 1}){}

TransformComponent::TransformComponent(Vec2 position, int rotation, Vec2 scale)
    :   position(position), rotation(rotation), scale(Vec2{1, 1}){}