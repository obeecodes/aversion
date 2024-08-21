#include "../../include/systems/inputSystem.h"
#include "../../include/systems/physicsSystem.h"
#include "../../include/systems/cameraSystem.h"
#include "../../include/systems/spriteSystem.h"
#include <SDL2/SDL_keyboard.h>
#include "../../include/core/window.h"
#include "../../include/core/renderer.h"
#include <iostream>
#include "../../include/entities/entityManager.h"
#include "../../include/core/game.h"

#include "../../include/systems/audioSystem.h"
#include "../../include/systems/eventManager.h"


InputSystem* InputSystem::instance = nullptr;

InputSystem::InputSystem() 
: inputComponentSystem(new Aurora::ECS<InputComponent>()),
  inputs(inputComponentSystem->getComponents()),
  inactiveIndex(inputComponentSystem->getInactiveIndex()),
  physics (PhysicsSystem::Instance()->getPhysics()), 
  sprites(SpriteSystem::Instance()->getSprites()),
  eventManager(EventManager::Instance())
  {

  this->keystates = SDL_GetKeyboardState(NULL);

  this->mousePosition = Vec2{};
  
  for(Uint32 i = 0; i < 3; i++){
    this->mouseButtonStates.push_back(false);
  }

}

InputSystem* InputSystem::Instance(){
  if(!instance){
    instance = new InputSystem();
  }
  return instance;
}

void InputSystem::createInputComponent(InputComponent input, Uint32 entityID){
  this->inputComponentSystem->createComponent(input, entityID);
}

bool InputSystem::isKeyDown(SDL_Scancode key){
  return (this->keystates != 0) && (this->keystates[key] == 1);
}

void InputSystem::update(){
  this->keystates = SDL_GetKeyboardState(NULL);
  
  SDL_Event event;

  while(SDL_PollEvent(&event)){
    switch(event.type){

    case SDL_KEYDOWN:
      //perform something when key is pressed
      keyPressed(event.key);
      break;
        
    case SDL_KEYUP:
      //perform something when key is released
      keyReleased(event.key);
      break;

    case SDL_MOUSEBUTTONDOWN:
      if(event.button.button == SDL_BUTTON_LEFT){
        this->mouseButtonStates[LEFT] = true;
      }
      if(event.button.button == SDL_BUTTON_MIDDLE){
        this->mouseButtonStates[MIDDLE] = true;
      }
      if(event.button.button == SDL_BUTTON_RIGHT){
        this->mouseButtonStates[RIGHT] = true;
      }
      break; 

    case SDL_MOUSEBUTTONUP:
      if(event.button.button == SDL_BUTTON_LEFT){
        this->mouseButtonStates[LEFT] = false;
      }
      if(event.button.button == SDL_BUTTON_MIDDLE){
        this->mouseButtonStates[MIDDLE] = false;
      }
      if(event.button.button == SDL_BUTTON_RIGHT){
        this->mouseButtonStates[RIGHT] = false;
      }
      break;

    case SDL_WINDOWEVENT:
      if(event.window.event == SDL_WINDOWEVENT_RESIZED){
        SDL_RenderSetIntegerScale(Renderer::Instance()->getRenderer(), SDL_TRUE);
        SDL_RenderSetScale(Renderer::Instance()->getRenderer(), 4, 4);
        SDL_RenderSetViewport(Renderer::Instance()->getRenderer(), NULL);
      }
      break;

    case SDL_MOUSEMOTION:
      this->mousePosition.x = event.motion.x;
      this->mousePosition.y = event.motion.y;
      break;

    case SDL_QUIT:
      Game::Instance()->setRunning(false);
      Renderer::Instance()->setRunning(false);
      Window::Instance()->setActive(false);
      Renderer::Instance()->clean();
      Window::Instance()->clean();
      break;

    }

  }

  for(Uint32 i = 0; i < this->inactiveIndex; i++){

    //EventSystem
    /*
    this->sprites[i].src_rect.y = (this->inputs[0].moveLeft) ? 2 * this->sprites[i].src_rect.h : this->sprites[i].src_rect.y;
    this->sprites[i].src_rect.y = (this->inputs[0].moveRight) ? 3 * this->sprites[i].src_rect.h : this->sprites[i].src_rect.y;
    this->sprites[i].src_rect.y = (this->inputs[0].moveUp) ? 0 : this->sprites[i].src_rect.y;
    this->sprites[i].src_rect.y = (this->inputs[0].moveDown) ? this->sprites[i].src_rect.h : this->sprites[i].src_rect.y;
    */
    
    eventManager->publish(Event{EventType::INPUT, this->inputComponentSystem->getEntityID(i), 0, this->inputs[i]});
   
  }

}

bool InputSystem::getMouseButtonState(int buttonNumber){
  return this->mouseButtonStates[buttonNumber];
}

Vec2& InputSystem::getMousePosition(){
  return mousePosition;
}

void InputSystem::clean(){
	for(Uint32 i = 0; i < 3; i++){
		this->mouseButtonStates.push_back(false);
	}
}

// event.repeat case 0 means key is not being held
void InputSystem::keyPressed(SDL_KeyboardEvent& event){

  for(int i = 0; i < this->inactiveIndex; i++){
    switch(Game::Instance()->getGameState()){
      case MAIN_MENU_STATE:
        handleMainMenuKeyPress(event);
        break;

      case TRANSITION_STATE:
        handleTransitionKeyPress(event);
        break;

      case PLAY_STATE:
        handlePlayKeyPress(event);
        break;

      case PAUSE_STATE:
        handlePauseKeyPress(event);
        break;

      case GAME_OVER_STATE:
        handleGameOverKeyPress(event);
        break;
    }
  }
}

void InputSystem::keyReleased(SDL_KeyboardEvent& event){
  switch(Game::Instance()->getGameState()){
    case MAIN_MENU_STATE:
      handleMainMenuKeyRelease(event);
      break;

    case TRANSITION_STATE:
      handleTransitionKeyRelease(event);
      break;

    case PLAY_STATE:
      handlePlayKeyRelease(event);
      break;

    case PAUSE_STATE:
      handlePauseKeyRelease(event);
      break;

    case GAME_OVER_STATE:
      handleGameOverKeyRelease(event);
      break;
  }

}

std::vector<InputComponent>& InputSystem::getInputs(){
  return this->inputs;
}

void InputSystem::handleMainMenuKeyPress(SDL_KeyboardEvent& event){

  for(int i = 0; i < this->inactiveIndex; i++){
    
    switch(event.repeat){

      case 0:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_ESCAPE:
            AudioSystem::Instance()->~AudioSystem();
            Game::Instance()->setRunning(false);
            Renderer::Instance()->setRunning(false);
            Window::Instance()->setActive(false);
            Renderer::Instance()->clean();
            Window::Instance()->clean();
            break;

          default:
            break;
        }
        break;

      default:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            //Placeholder code
            break;
          default:
            break;
        }
        break;

    }
  }
  
}

void InputSystem::handleTransitionKeyPress(SDL_KeyboardEvent& event){

  for(int i = 0; i < this->inactiveIndex; i++){
    
    switch(event.repeat){

      case 0:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            //Placeholder code
            break;
          break;
          default:
            break;
        }
        break;

      default:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            //Placeholder code
            break;
          default:
            break;
        }
        break;

    }
  }
  
}

void InputSystem::handlePlayKeyPress(SDL_KeyboardEvent& event){
  for(int i = 0; i < this->inactiveIndex; i++){
    switch(event.repeat){

      case 0:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            inputs[i].moveUp = true;
            break;
          case SDL_SCANCODE_A:
            inputs[i].moveLeft = true;
            break;
          case SDL_SCANCODE_S:
            inputs[i].moveDown = true;
            break;
          case SDL_SCANCODE_D:
            inputs[i].moveRight = true;
            break;
          case SDL_SCANCODE_ESCAPE:
            Game::Instance()->setGameState(MAIN_MENU_STATE); 
            Game::Instance()->popGameState();
            break;
          case SDL_SCANCODE_H:
            //inputs[i].createEntity = true;
            break;
          case SDL_SCANCODE_MINUS:
            CameraSystem::Instance()->zoomOut();
          break;
          case SDL_SCANCODE_EQUALS:
            CameraSystem::Instance()->zoomIn();
          break;
          case SDL_SCANCODE_B:
            AudioSystem::Instance()->playSample(0);
          default:
            break;
        }
        break;

      default:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            inputs[i].moveUp = true;
            break;
          case SDL_SCANCODE_A:
            inputs[i].moveLeft = true;
            break;
          case SDL_SCANCODE_S:
            inputs[i].moveDown = true;
            break;
          case SDL_SCANCODE_D:
            inputs[i].moveRight = true;
            break;
          case SDL_SCANCODE_H:
            inputs[i].createEntity = false;
            break;
          default:
            break;
        }
        break;

    }
  }
}

void InputSystem::handlePauseKeyPress(SDL_KeyboardEvent& event){

  for(int i = 0; i < this->inactiveIndex; i++){
    
    switch(event.repeat){

      case 0:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            //Placeholder code
            break;
          break;
          default:
            break;
        }
        break;

      default:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            //Placeholder code
            break;
          default:
            break;
        }
        break;

    }
  }
  
}

void InputSystem::handleGameOverKeyPress(SDL_KeyboardEvent& event){

  for(int i = 0; i < this->inactiveIndex; i++){
    
    switch(event.repeat){

      case 0:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            //Placeholder code
            break;
          break;
          default:
            break;
        }
        break;

      default:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            //Placeholder code
            break;
          default:
            break;
        }
        break;

    }
  }
  
}

void InputSystem::handleMainMenuKeyRelease(SDL_KeyboardEvent& event){
  
  for(Uint32 i = 0; i < this->inactiveIndex; i++){
    switch(event.repeat){
      case 0:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            //Placeholder
            break;
          default:
            break;
        }
        break;

      // SDL2 does not perform repeated key releases
      default:
        break;
    }
  }
  
}

void InputSystem::handleTransitionKeyRelease(SDL_KeyboardEvent& event){
  
  for(Uint32 i = 0; i < this->inactiveIndex; i++){
    switch(event.repeat){
      case 0:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            //Placeholder
            break;
          default:
            break;
        }
        break;

      // SDL2 does not perform repeated key releases
      default:
        break;
    }
  }
  
}

void InputSystem::handlePlayKeyRelease(SDL_KeyboardEvent& event){

  for(Uint32 i = 0; i < this->inactiveIndex; i++){
    switch(event.repeat){
      case 0:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            inputs[i].moveUp = false;
            break;
          case SDL_SCANCODE_A:
            inputs[i].moveLeft = false;
            break;
          case SDL_SCANCODE_S:
            inputs[i].moveDown = false;
            break;
          case SDL_SCANCODE_D:
            inputs[i].moveRight = false;
            break;
          case SDL_SCANCODE_H:
            inputs[i].createEntity = false;
            break;
          default:
            break;
        }
        break;

      // SDL2 does not perform repeated key releases
      default:
        break;
    }
  }
  
}

void InputSystem::handlePauseKeyRelease(SDL_KeyboardEvent& event){
  
  for(Uint32 i = 0; i < this->inactiveIndex; i++){
    switch(event.repeat){
      case 0:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            //Placeholder
            break;
          default:
            break;
        }
        break;

      // SDL2 does not perform repeated key releases
      default:
        break;
    }
  }
  
}

void InputSystem::handleGameOverKeyRelease(SDL_KeyboardEvent& event){
  
  for(Uint32 i = 0; i < this->inactiveIndex; i++){
    switch(event.repeat){
      case 0:
        switch(event.keysym.scancode){
          case SDL_SCANCODE_W:
            //Placeholder
            break;
          default:
            break;
        }
        break;

      // SDL2 does not perform repeated key releases
      default:
        break;
    }
  }
  
}
