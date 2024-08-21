#include "../../include/components/spriteComponent.h"

SpriteComponent::SpriteComponent(){
  this->atl = BLANK;
  this->src_rect = SDL_Rect();
  this->dest_rect = SDL_Rect();
}

SpriteComponent::SpriteComponent(Atlas atl, SDL_Rect src_rect, SDL_Rect dest_rect)
  : atl(atl), src_rect(src_rect), dest_rect(dest_rect){}
