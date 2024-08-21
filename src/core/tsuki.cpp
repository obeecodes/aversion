#include "../../include/core/tsuki.h"
#include "../../include/systems/physicsSystem.h"

Tsuki* Tsuki::instance = nullptr;

Tsuki* Tsuki::Instance(){
    if(!instance){
        instance = new Tsuki();
    }
    return instance;
}

Tsuki::Tsuki()
:   eventManager(EventManager::Instance()){
    L = luaL_newstate();
    luaL_openlibs(L);
    registerFunctions();
}

Tsuki::~Tsuki(){
    lua_close(L);
}

void Tsuki::setPupPosition(Vec2 pos){
    lua_pushnumber(L, pos.x);
    lua_setglobal(L, "pupX");

    lua_pushnumber(L, pos.y);
    lua_setglobal(L, "pupY");
}

void Tsuki::pushVec2(Vec2 position){
    lua_pushnumber(L, position.x);
    lua_setglobal(L, "playerX");

    lua_pushnumber(L, position.y);
    lua_setglobal(L, "playerY");
}

void Tsuki::pushEntityId(Uint32 entityID){
    lua_pushnumber(L, entityID);
    lua_setglobal(L, "entityID");

}

void Tsuki::callScript(const char* file){
    
    if (luaL_dofile(L, file)/*luaL_loadfile(L, file) || lua_pcall(L, 0, 0, 0)*/) {
        std::cerr << "Error loading or running Lua script: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        return;
    }
}

int Tsuki::l_setEntityVelocity(lua_State* L){
    
    Uint32 id = luaL_checkinteger(L, 1);
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);

    Tsuki::Instance()->setEntityVelocity(id, x, y);

    return 0;
}

void Tsuki::setEntityVelocity(Uint32 id, float x, float y){
    // publish event
    Vec2 vel = Vec2{x, y};
    vel.normalize();

    lua_getglobal(L, "pupX");
    double pupX = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getglobal(L, "pupY");
    double pupY = lua_tonumber(L, -1);
    lua_pop(L, 1);

    // Perform the subtraction
    pupX += vel.x*2;
    pupY += vel.y*2;

    // Push the updated values back to Lua
    lua_pushnumber(L, pupX);
    lua_setglobal(L, "pupX");

    lua_pushnumber(L, pupY);
    lua_setglobal(L, "pupY");

    eventManager->publish(Event{EventType::UPDATE_VELOCITY , (Uint32)id, vel});

}

void Tsuki::registerFunctions(){
    lua_register(L, "setEntityVelocity", Tsuki::l_setEntityVelocity);
}


