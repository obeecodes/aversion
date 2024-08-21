#include "../../include/components/cameraComponent.h"

CameraComponent::CameraComponent()
    : position(Vec2{}), width(0), height(0){}

CameraComponent::CameraComponent(Vec2 position, int width, int height)
    : position(Vec2{}), width(width), height(height){}
