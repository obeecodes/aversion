#include "../../../include/core/gameStates/mainMenuState.h"
#include "../../../include/core/gameStates/playState.h"
#include "../../../include/systems/inputSystem.h"
#include "../../../include/core/renderer.h"
#include <SDL2/SDL_stdinc.h>
#include "../../../include/core/widgets/mainMenuButton.h"
#include "../../../include/core/game.h"
#include "iostream"

 MainMenuState:: MainMenuState(){
  
  this->buttons.push_back(new MainMenuButton(0, 20 * 0 , 128, 32, " Play! ", [](){ Game::Instance()->setGameState(PLAY_STATE); Game::Instance()->pushGameState(new PlayState());}));
  this->buttons.push_back(new MainMenuButton(0, 20 * 1 , 128, 32, " Settings ", [](){std::cout << "Clicked Settings!\n";}));
  this->buttons.push_back(new MainMenuButton(0, 20 * 2 , 128, 32, " Credits ", [](){std::cout << "Clicked Credits!\n";}));

  //this->buttons.push_back(new Button(0, 20 * 0 , 32, 32, [](){ std::cout << "だめ！\n";}));

}

 MainMenuState::~ MainMenuState(){
  for(Button* button : this->buttons){
    delete button;
  }
  this->buttons.clear();
}


void  MainMenuState::update(){

  InputSystem::Instance()->update();

}

void  MainMenuState::render(){

  Renderer::Instance()->renderClear();

  for(Uint32 i = 0; i < buttons.size(); i++){
    buttons[i]->update();
    buttons[i]->render();
  }

  Renderer::Instance()->renderPresent();

}