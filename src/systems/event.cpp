#include "../../include/systems/event.h"

Event::Event(EventType type, Uint32 entityA, Uint32 entityB, EventData data)
    : type(type), entityA(entityA), entityB(entityB), data(data){}

Event::Event(EventType type, Uint32 entityA, Uint32 entityB)
    : type(type), entityA(entityA), entityB(entityB){}

Event::Event(EventType type, Uint32 entityA, EventData data)
    : type(type), entityA(entityA), entityB(0), data(data){}
