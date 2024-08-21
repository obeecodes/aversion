#include "../../include/systems/quadtree.h"


Quadtree::Quadtree(SDL_Rect region)
    : region(region){
    this->level = 1;
}

Quadtree::Quadtree(Uint32 level, SDL_Rect region)
    : level(level), region(region){
    for(Uint32 i = 0; i < 4; i++){
        nodes[i] = nullptr;
    }
}

void Quadtree::clear(){
    regionComponents.clear();

    for(Uint32 i = 0; i < 4; i++){
        if(nodes[i] != nullptr){
            nodes[i]->clear();
            delete nodes[i];
            nodes[i] = nullptr;
        }
    }
}

void Quadtree::insert(ColliderComponent colliderComponent){
    if(nodes[0] != nullptr){
        Uint32 index = getIndex(colliderComponent.rect);

        if(index != SDL_MAX_UINT32){
            nodes[index]->insert(colliderComponent);
            return;
        }
    }

    regionComponents.push_back(colliderComponent);

    if(regionComponents.size() > MAX_OBJECTS && level < MAX_DEPTH){
        if(nodes[0] == nullptr){
            split();
        }

        Uint32 i = 0;
        while(i < regionComponents.size()){
            Uint32 index = getIndex(regionComponents[i].rect);
            if(index != SDL_MAX_UINT32){
                nodes[index]->insert(regionComponents[i]);
                regionComponents.erase(regionComponents.begin() + i);
            } else {
                i++;
            }
        }
    }
}

void Quadtree::remove(ColliderComponent colliderComponent){
    Uint32 index = getIndex(colliderComponent.rect);
    if(index != SDL_MAX_UINT32 && nodes[0] != nullptr){
        nodes[index]->remove(colliderComponent);
    } else {
        //auto it = std::find(regionComponents.begin(), regionComponents.end(), colliderComponent);
        //if(it != regionComponents.end()){
          //  regionComponents.erase(it);
     //   }
    }
}

void Quadtree::retrieve(std::vector<ColliderComponent>& nearComponents, const SDL_Rect& rect) const{
    Uint32 index = getIndex(rect);
    if(index != SDL_MAX_UINT32 && nodes[0] != nullptr){
        nodes[index]->retrieve(nearComponents, rect);
    }

    nearComponents.insert(nearComponents.end(), regionComponents.begin(), regionComponents.end());
}

void Quadtree::split(){
    int subWidth = region.w / 2;
    int subHeight = region.h / 2;
    int x = region.x;
    int y = region.y;

    nodes[0] = new Quadtree(level + 1, SDL_Rect{ x + subWidth, y, subWidth, subHeight});
    nodes[1] = new Quadtree(level + 1, SDL_Rect{ x, y, subWidth, subHeight});
    nodes[2] = new Quadtree(level + 1, SDL_Rect{ x, y + subHeight, subWidth, subHeight});
    nodes[3] = new Quadtree(level + 1, SDL_Rect{ x + subWidth, y + subHeight, subWidth, subHeight});
}


Uint32 Quadtree::getIndex(const SDL_Rect& rect) const {
    Uint32 index = SDL_MAX_UINT32;
    int verticalMidpoint = region.x + (region.w / 2);
    int horizontalMidpoint = region.y + (region.h / 2);

    bool topQuads = (rect.y < horizontalMidpoint && rect.y + rect.h < horizontalMidpoint);
    bool bottomQuads = (rect.y > horizontalMidpoint);

    if(rect.x < verticalMidpoint && rect.x + rect.w < verticalMidpoint){
        if(topQuads){
            index = 1;
        } else if (bottomQuads){
            index = 2;
        }
    } else if(rect.x > verticalMidpoint){
        if(topQuads){
            index = 0;
        }else if(bottomQuads){
            index = 3;
        }
    }
    return index;
}