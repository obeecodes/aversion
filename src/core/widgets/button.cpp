#include "../../../include/core/widgets/button.h"
#include "../../../include/core/renderer.h"
#include "../../../include/systems/spriteSystem.h"
#include "../../../include/systems/inputSystem.h"
#include <iostream>

Button::Button(int x, int y, int w, int h, std::function<void(void)> callback){
    // what portion of texture to use
    //this is a gray color from the texture 
    this->src = SDL_Rect{0, 0, 128, 32};

    // destination to draw button in renderer
    this->dest = SDL_Rect{x, y, w, h};
    this->original = this->dest;

    this->callback = callback;
    this->pressed = false;
    this->released = true;
    bool hovered = false;

    this->pressedPosition = Vec2{};
    this->selected = false;

}

void Button::render(){

    SDL_RenderCopy(Renderer::Instance()->getRenderer(), SpriteSystem::Instance()->getTexture(UI), &this->src, &dest);

    //pleasant green color
    SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0x7f, 0x9c, 0x80, 0xff);

}

void Button::update(){
    
    bool leftMouseButton = InputSystem::Instance()->getMouseButtonState(LEFT);

    if(leftMouseButton && !selected){
        pressedPosition = InputSystem::Instance()->getMousePosition();
        selected = true;
        pressed = true;
        released = false;
    } else if(!leftMouseButton && pressed){
        pressed = false;
        released = true;
        selected = false;
    } else if(!leftMouseButton){
        released = true;
    }

    if(isHovered()){
        
        hovered = true;
        
        if (released && selectedButton()){
            callback();
            released = false;
            pressed = false;
        }

    }else{
        hovered = false;
    }
    if(released)
        pressedPosition = Vec2{};
    updateButton();
}

bool Button::isHovered(){
    Vec2& mousePosition = InputSystem::Instance()->getMousePosition();
    
    return mousePosition.x > dest.x && mousePosition.y > dest.y &&
        mousePosition.x < dest.x + dest.w && mousePosition.y < dest.y + dest.h;
}

void Button::updateButton(){

    if (hovered && selectedButton() && InputSystem::Instance()->getMouseButtonState(LEFT)){
        this->dest.x = original.x + 1;
        this->dest.y = original.y + 1;
        this->dest.w = original.w - 2;
        this->dest.h = original.h -2;

        this->src.y = 32; //button should be dark gray
    }else if(hovered){
        // hovered button
        // update color to dark gray
        this->src.y = 32;
        this->dest = original;
    } else {
        // original button
        this->src.y = 0;
        this->dest = original;
    }

}

bool Button::selectedButton(){
    SDL_Point point = SDL_Point{static_cast<int>(pressedPosition.x), static_cast<int>(pressedPosition.y)};
    return SDL_PointInRect(&point, &original);

}
