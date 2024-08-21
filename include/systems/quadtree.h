#pragma once

#include <vector>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_rect.h>
#include "../components/colliderComponent.h"

class Quadtree{
public:

    Quadtree(SDL_Rect region);
    Quadtree(Uint32 level, SDL_Rect region);
    void clear();
    void insert(ColliderComponent collider);
    void remove(ColliderComponent collider);
    void retrieve(std::vector<ColliderComponent>& nearComponents, const SDL_Rect& rect) const;

private:
    const Uint32 MAX_DEPTH = 10;
    const Uint32 MAX_OBJECTS = 8;
    Uint32 level;

    SDL_Rect region;
    std::vector<ColliderComponent> regionComponents;
    Quadtree* nodes[4];

    void split();

    Uint32 getIndex(const SDL_Rect& rect) const;


};