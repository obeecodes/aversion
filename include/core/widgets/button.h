#pragma once

#include "../../SDL2/SDL_ttf.h"
#include <SDL2/SDL.h>
#include <string>
#include <functional>
#include "../../components/vec2.h"

class Button{
public:
    Button(int x, int y, int w, int h, std::function<void(void)> callback);
    virtual void render();
    virtual void update();

protected:
    SDL_Rect src;
    SDL_Rect dest;
    SDL_Rect original;

    std::function<void(void)> callback;

    virtual void updateButton();

    bool isHovered();
    bool hovered;
    bool pressed;
    bool released;

    /* on MacOS, if you click and hold the red window close button,
    * move your pointer outside the button, and move your pointer 
    * back into the button and release, the window closes. 
    * If you do the same action, but release inside of another button, 
    * nothing happens. 
    * The following is useful in allowing that in my game
    */

    Vec2 pressedPosition;
    bool selected;
    bool selectedButton();

};