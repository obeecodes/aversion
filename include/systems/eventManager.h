#pragma once

#include "event.h"
#include <functional>
#include <queue>

class EventManager{
public:
    static EventManager* Instance();
    void subscribe(EventType type, std::function<void(const Event event)> handler);
    void publish(const Event event);
    void processEvents();

private:
    static EventManager* instance;
    std::unordered_map<EventType, std::vector<std::function<void(const Event event)>>> handlers;
    std::queue<Event> events;
    EventManager();
};