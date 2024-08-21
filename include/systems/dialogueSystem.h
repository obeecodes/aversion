#pragma once 
#include "../components/dialogueComponent.h"
#include "../SDL2/SDL_ttf.h"
#include <vector>
#include <SDL2/SDL.h>
#include "../core/ecs.h"
#include "eventManager.h"

class DialogueSystem{
public:
  static DialogueSystem* Instance();
  void update();
  void createDialogueComponent(DialogueComponent dialogue, Uint32 entityID);
  void displayText();
  void setFPS(float fps);
  TTF_Font* getFont();

private:
  static DialogueSystem* instance;
  Aurora::ECS<DialogueComponent>* dialogueComponentSystem;
  std::vector<DialogueComponent>& dialogues;
  Uint32& inactiveIndex;
  EventManager* eventManager;
  DialogueSystem();

  TTF_Font* font; 

  float fps;

};