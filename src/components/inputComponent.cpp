#include "../../include/components/inputComponent.h"

InputComponent::InputComponent(bool moveUp, bool moveDown, bool moveLeft, bool moveRight, bool createEntity)
    : moveUp(moveUp), moveDown(moveDown), moveLeft(moveLeft), moveRight(moveRight), createEntity(createEntity){}