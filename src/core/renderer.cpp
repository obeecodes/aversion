#include "../../include/core/renderer.h"
#include "../../include/core/window.h"
#include "../../include/systems/inputSystem.h"
#include <SDL2/SDL.h>
#include "../../include/SDL2/SDL_image.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include "../../include/systems/renderSystem.h"
#include "sys/sysctl.h"
#include <iostream>

Renderer* Renderer::instance = nullptr;

Renderer::Renderer(){
  this->running = true;
  this->renderer = NULL;
}

Renderer* Renderer::Instance(){
  if(!instance){
    instance = new Renderer();
  }
  return instance;
}

bool Renderer::initRenderer(){
    
  if(Window::Instance()->getWindow() != NULL){
    //creates renderer
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    if(isM1iMac()){
      this->renderer = SDL_CreateRenderer(Window::Instance()->getWindow(), -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC); //SDL_RENDERER_SOFTWARE prevents rendering glitch somehow
    } else{
      this->renderer = SDL_CreateRenderer(Window::Instance()->getWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //SDL_RENDERER_SOFTWARE prevents rendering glitch somehow
    }
    //SDL_SetRenderScale(this->renderer, 4, 4);
    SDL_RenderSetLogicalSize(this->renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_RenderSetIntegerScale(Renderer::Instance()->getRenderer(), SDL_TRUE);
    SDL_RenderSetScale(Renderer::Instance()->getRenderer(), 4, 4);
    SDL_RenderSetViewport(Renderer::Instance()->getRenderer(), NULL);
  }else{
    SDL_Log("Failed to initialize SDL2 window: %s", SDL_GetError());
    SDL_Quit();
    return false;
  }

  //sets draw color
  //SDL_SetRenderDrawColor(this->renderer, 216, 222, 233, 255); 
  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); 

  //clears window
  SDL_RenderClear(this->renderer);
  
  return true;
}

void Renderer::render(){

}

void Renderer::renderClear(){
  SDL_RenderClear(this->renderer);
}

void Renderer::renderPresent(){
  SDL_RenderPresent(this->renderer);
}

void Renderer::update(){}

void Renderer::clean(){
  this->running = false;
  SDL_DestroyRenderer(this->renderer);
}

void Renderer::setRunning(bool running){
  this->running = running;
}

bool Renderer::getRunning(){
  return this->running;
}

void Renderer::setRenderer(SDL_Renderer* renderer){
  this->renderer = renderer;
}

SDL_Renderer* Renderer::getRenderer(){
  return this->renderer;
}

//SDL_RENDERER_ACCELERATED causes tile glitches on M1 iMac
//SDL_RENDERER_SOFTWARE causes tile glitches on M2 Macbook
//Use SDL_RENDERER_SOFTWARE for M1 iMAC and SDL_RENDERER_ACCELERATED for M2 Macbook
bool Renderer::isM1iMac() {
    int mib[2];
    size_t len;
    char *machine;

    mib[0] = CTL_HW;
    mib[1] = HW_MODEL;
    sysctl(mib, 2, NULL, &len, NULL, 0);
    machine = (char *)malloc(len);
    sysctl(mib, 2, machine, &len, NULL, 0);

    bool isM1 = strstr(machine, "iMac") != NULL && strstr(machine, "Mac") != NULL;

    free(machine);

    return isM1;
}

