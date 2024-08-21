#include "../../include/systems/dialogueSystem.h"
#include "../../include/core/renderer.h"
#include <iostream>

DialogueSystem* DialogueSystem::instance = nullptr;

DialogueSystem* DialogueSystem::Instance(){
  if(!instance){
    instance = new DialogueSystem();
  }
  return instance;
}

DialogueSystem::DialogueSystem()
: dialogueComponentSystem(new Aurora::ECS<DialogueComponent>()),
  dialogues(dialogueComponentSystem->getComponents()),
  inactiveIndex(dialogueComponentSystem->getInactiveIndex()),
  eventManager(EventManager::Instance()){
  this->font = TTF_OpenFont("assets/fonts/Hack_Regular_Nerd_Font_Complete_Mono.ttf", 12);


}

void DialogueSystem::createDialogueComponent(DialogueComponent dialogue, Uint32 entityID){
  this->dialogueComponentSystem->createComponent(dialogue, entityID);
}

void DialogueSystem::update(){
  displayText();
  for(int i = 0; i < this->inactiveIndex; i++){

  }
}

void DialogueSystem::displayText(){
  if(!this->font){
    std::cout << "FONT NOT FOUND" << std::endl;
    exit(1);
  }

  SDL_Color black = {0, 0 , 0, 255};
  SDL_Surface* textSurface = TTF_RenderText_Solid(font, ("FPS: " + std::to_string(fps)).c_str(), black);
  SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Renderer::Instance()->getRenderer(), textSurface);
  SDL_FreeSurface(textSurface);
  
  int w;
  int h;

  SDL_QueryTexture(textTexture, NULL, NULL, &w, &h);

  SDL_Rect dest = {0, 0, w, h};
  SDL_RenderCopy(Renderer::Instance()->getRenderer(), textTexture, NULL, &dest);

}

void DialogueSystem::setFPS(float fps){
  this->fps = fps;
}

TTF_Font* DialogueSystem::getFont(){
  return this->font;
}
