#pragma once 

#include <vector>
#include <memory>
#include <utility>
#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>
#include "../components/inputComponent.h"
#include "../components/physicsComponent.h"
#include "../components/spriteComponent.h"
#include "../core/ecs.h"
#include "../components/vec2.h"
#include "eventManager.h"

enum mouse_buttons{
  LEFT = 0,
  MIDDLE = 1,
  RIGHT = 2,
};

class InputSystem{
public:
  static InputSystem* Instance();
  void update();
  void clean();

  bool isKeyDown(SDL_Scancode key);
  void keyPressed(SDL_KeyboardEvent& event);
  void keyReleased(SDL_KeyboardEvent& event);

  bool getMouseButtonState(int buttonNumber);
  Vec2& getMousePosition();
  void createInputComponent(InputComponent input, Uint32 entityID);
  std::vector<InputComponent>& getInputs();
  
  // Key Press
  void handleMainMenuKeyPress(SDL_KeyboardEvent& event);
  void handleTransitionKeyPress(SDL_KeyboardEvent& event);
  void handlePlayKeyPress(SDL_KeyboardEvent& event);
  void handlePauseKeyPress(SDL_KeyboardEvent& event);
  void handleGameOverKeyPress(SDL_KeyboardEvent& event);

  // Key Release
  void handleMainMenuKeyRelease(SDL_KeyboardEvent& event);
  void handleTransitionKeyRelease(SDL_KeyboardEvent& event);
  void handlePlayKeyRelease(SDL_KeyboardEvent& event);
  void handlePauseKeyRelease(SDL_KeyboardEvent& event);
  void handleGameOverKeyRelease(SDL_KeyboardEvent& event);


private:
  InputSystem();
  ~InputSystem();

  static InputSystem* instance;

  Aurora::ECS<InputComponent>* inputComponentSystem;
  const Uint8* keystates;
  std::vector<bool> mouseButtonStates;
  Vec2 mousePosition;
  std::vector<InputComponent>& inputs;
  std::vector<PhysicsComponent>& physics;
  std::vector<SpriteComponent>& sprites; 
  Uint32& inactiveIndex;
  EventManager* eventManager;
  
};
