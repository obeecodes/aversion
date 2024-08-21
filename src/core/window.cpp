#include "../../include/core/window.h"
#include <SDL2/SDL.h>

Window* Window::instance = nullptr;

Window* Window::Instance(){
    if(!instance){
        instance = new Window();
    }
    return instance;
}

Window::Window(){
  this->title = NULL;
  this->window = NULL;
  this->active = true;
}

bool Window::initWindow(){
  //initialize SDL
  if(SDL_Init(/*SDL_INIT_AUDIO | */SDL_INIT_VIDEO | SDL_INIT_EVENTS) >= 0){
    //create window
    this->window = SDL_CreateWindow(this->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_DisplayMode displaymode;
    SDL_GetCurrentDisplayMode(0, &displaymode);
    //SDL_SetWindowSize(window, displaymode.w, displaymode.h);

  }else{
    SDL_Log("Failed to initialize SDL2: %s", SDL_GetError());
    SDL_Quit();
    return false;
  }

  return true;
}

void Window::setTitle(const char* title){
    this->title = title;
}

const char* Window::getTitle() const{
    return this->title;
}

void Window::setActive(bool active){
    this->active = active;
}

bool Window::getActive() const{
    return this->active;
}

void Window::setWindow(SDL_Window* const window){
  this->window = window;
}

SDL_Window* Window::getWindow() const{
    return this->window;
}

void Window::clean(){
  this->active = false;
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}