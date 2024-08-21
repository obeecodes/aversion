#include "../../include/components/dialogueComponent.h"

DialogueComponent::DialogueComponent(){
  this->text = nullptr;
}

DialogueComponent::DialogueComponent(char* text)
  : text(text){}

