#pragma once 

#include "../../external/lua/include/lua.hpp"
#include "../components/vec2.h"
#include "../systems/eventManager.h"
#include <SDL2/SDL_stdinc.h>

class Tsuki {
public:
    static Tsuki* Instance();
    void pushVec2(Vec2 position);
    void pushEntityId(Uint32 entityID);
    void setPupPosition(Vec2 pos);
    void callScript(const char* file);

    // lua wrapper
    static int l_setEntityVelocity(lua_State* L);

private:
    Tsuki();
    ~Tsuki();
    static Tsuki* instance;
    lua_State* L;
    void registerFunctions();
    EventManager* eventManager;

    // callable from lua wrapper
    void setEntityVelocity(Uint32 id, float x, float y);

};

