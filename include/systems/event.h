#pragma once

#include <SDL2/SDL_rect.h>
#include <variant>
#include <vector>
#include <utility>
#include "../components/vec2.h"
#include "../components/inputComponent.h"
#include "../components/healthComponent.h"

enum class EventType {
    WALL_COLLISION,
    COLLISION,
    DAMAGE,
    INPUT,
    CREATE_COMPONENT,
    PLAYER_MOVEMENT,
    UPDATE_VELOCITY,
    UPDATE_PLAYER_HEALTH,
    MOVE_INTENT,
    MOVE_SUCCESS,
    MOVEMENT,
    SPRITE_UPDATE,
    SPRITE_PHYSICS_UPDATE,
    CAMERA_UPDATE,
    
};

using EventData = std::variant<Uint32, Vec2, InputComponent, HealthComponent, std::pair<SDL_Rect, SDL_Rect> >;

struct Event{
    EventType type;
    Uint32 entityA;
    Uint32 entityB;

    EventData data;

    Event(EventType type, Uint32 entityA, Uint32 entityB, EventData data);
    Event(EventType type, Uint32 entityA, Uint32 entityB);
    Event(EventType type, Uint32 entityA, EventData data);
};

