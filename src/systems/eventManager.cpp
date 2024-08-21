#include "../../include/systems/eventManager.h"

EventManager* EventManager::instance = nullptr;

EventManager* EventManager::Instance(){
    if(!instance){
        instance = new EventManager();
    }
    return instance;
}

EventManager::EventManager(){}

void EventManager::subscribe(EventType type, std::function<void(const Event event)> handler){
    this->handlers[type].push_back(handler);
}

void EventManager::publish(const Event event){
    //for(const auto& handler : handlers[event.type]){
      //  handler(event);
    //}
    events.push(event);
}

void EventManager::processEvents(){
    while(!events.empty()){
        Event event = events.front();
        events.pop();

        // Check if the event type has handlers
        auto it = handlers.find(event.type);
        if (it != handlers.end()) {
            for (const auto& handler : it->second) {
                handler(event);
            }
        }
    }
}