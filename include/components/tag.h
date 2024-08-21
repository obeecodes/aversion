#pragma once

#include <SDL2/SDL_stdinc.h>

enum class TagType: Uint8 {
    TILE,
};

struct Tag{};
struct TileTag : public Tag{};
