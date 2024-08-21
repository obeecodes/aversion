#pragma once
#include <SDL2/SDL_render.h>

enum Atlas{
  PLAYER = 1 << 0,
  PUP = 1 << 1,
  WORLD = 1 << 2,
  UI = 1 << 3,
  ENEMY = 1 << 4,
  BLANK = 1 << 5,
  RAIN = 1 << 6,
};

struct SpriteComponent{
  //sprite system can hold sprite atlas
  Atlas atl;
  SDL_Rect src_rect;
  SDL_Rect dest_rect;
  SpriteComponent();
  SpriteComponent(Atlas atl, SDL_Rect src_rect, SDL_Rect dest_rest);
};
