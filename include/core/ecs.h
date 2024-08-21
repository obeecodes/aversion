#pragma once

#include <vector>
#include <unordered_set>
#include <SDL2/SDL.h>

namespace Aurora{
    template <typename T>
    class ECS{
    public:
        ECS();
        void createComponent(T component, Uint32 entityID);
        void destroyComponent(Uint32 entityID);

        std::vector<T>& getComponents();
        T getEntityComponent(Uint32 entityID);
        Uint32& getInactiveIndex();
        Uint32 getEntityIndex(Uint32 entityID);
        Uint32 getEntityID(Uint32 index);
        bool hasComponent(Uint32 entityID);

    private:
        std::vector<T> components;
        std::vector<Uint32> denseComponents;
        std::vector<Uint32> sparseComponents;
        Uint32 inactiveIndex;
        std::unordered_set<Uint32> active;
    };
}