#include "../../../include/core/widgets/mainMenuButton.h"
#include "../../../include/core/widgets/button.h"
#include "../../../include/core/renderer.h"
#include "../../../include/systems/spriteSystem.h"
#include "../../../include/systems/inputSystem.h"
#include <iostream>

MainMenuButton::MainMenuButton(int x, int y, int w, int h, const std::string& text, std::function<void(void)> callback) 
: Button(x, y, w, h, callback), text(text){
    // what portion of texture to use
    //this->src = SDL_Rect{0, 0, 128, 32};

    // destination to draw button in renderer
    //this->dest = SDL_Rect{x, y, w, h};

    this->offset = y;
    this->text = text;
    this->textColor = SDL_Color{0, 0, 0, 255};


    bool hovered = false;
    this->textTexture = nullptr;
    initButton();

}

void MainMenuButton::render(){

    SDL_RenderCopy(Renderer::Instance()->getRenderer(), SpriteSystem::Instance()->getTexture(UI), &this->src, &dest);
    SDL_RenderCopy(Renderer::Instance()->getRenderer(), this->textTexture, nullptr, &dest);

    //SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0xff, 0xff, 0xff, 0xff);
    //SDL_RenderRect(Renderer::Instance()->getRenderer(), &textDest);

    SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 0x7f, 0x9c, 0x80, 0xff);

}

void MainMenuButton::update(){
    SDL_Rect viewport;
    SDL_RenderGetViewport(Renderer::Instance()->getRenderer(), &viewport);
    this->dest = { viewport.x + (viewport.w - BUTTON_WIDTH) / 2, offset + viewport.y + (viewport.h - BUTTON_HEIGHT) / 2, BUTTON_WIDTH, BUTTON_HEIGHT};
    this->original = this->dest;
    
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


void MainMenuButton::initButton(){

    TTF_Font* font = TTF_OpenFont("assets/fonts/pixel.ttf", 10);

    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    this->textTexture = SDL_CreateTextureFromSurface(Renderer::Instance()->getRenderer(), textSurface);
    
    TTF_CloseFont(font);

    SDL_FreeSurface(textSurface);
}