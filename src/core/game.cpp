#include "../../include/core/game.h"
#include <SDL2/SDL.h>
#include "../../include/core/window.h"
#include "../../include/systems/inputSystem.h"
#include "../../include/core/renderer.h"
#include "../../include/core/TMXParser.h"
#include "../../include/core/blackBoard.h"
#include "../../include/systems/aiSystem.h"
#include "../../include/systems/audioSystem.h"
#include "../../include/systems/cameraSystem.h"
#include "../../include/systems/collisionSystem.h"
#include "../../include/systems/dialogueSystem.h"
#include "../../include/systems/healthSystem.h"
#include "../../include/systems/lightSystem.h"
#include "../../include/systems/physicsSystem.h"
#include "../../include/entities/entityManager.h"
#include "../../include/systems/renderSystem.h"
#include "../../include/systems/spriteSystem.h"
#include "../../include/core/gameStates/mainMenuState.h"
#include "../../include/core/gameStates/transitionState.h"
#include "../../include/core/gameStates/playState.h"
#include "../../include/core/gameStates/pauseState.h"
#include "../../include/core/gameStates/gameOverState.h"
#include "../../include/core/jsonParser.h"
#include "../../include/core/map.h"

#include <iostream>

#include "../../include/SDL2/SDL_ttf.h"

//int count = 0;


Game* Game::instance = nullptr;

Game* Game::Instance(){
  if(!instance){
    instance = new Game();
  }
  return instance;
}

Game::Game(){
  TTF_Init();
  this->running = true;
  this->gameState = MAIN_MENU_STATE;

  Window::Instance()->setTitle("Aversion");
  Window::Instance()->initWindow();
  Renderer::Instance()->initRenderer();
  SpriteSystem::Instance()->initTextures();
  AudioSystem::Instance()->initAudio();
  TMXParser::Instance()->initTMXParser("assets/map/arena.tmx");
  JSONParser::Instance()->initJSONParser("assets/archetypes/entities.json");
  BlackBoard::Instance()->initBlackBoard();
  
  //Initialize player
  
  
  //EntityManager::Instance()->createPlayer();
  //instead of createPlayer(), loadup all game entities
  EntityManager::Instance()->createEntities();


  Map::Instance()->initMap();

  
  this->gameStates.push(new MainMenuState());
  this->gameState = MAIN_MENU_STATE; 
  /*this->gameStates.push(new PlayState());
  this->gameState = PLAY_STATE;
*/
  this->fps = 60;
  this->cycleTime = std::chrono::duration<float>{1.0f / fps};
  this->elapsedSeconds = std::chrono::duration<float>{0.0f};
  this->accumulatedSeconds = 0.0f;

}

Game::~Game(){
  SDL_QuitSubSystem(SDL_INIT_AUDIO);

}

bool Game::getRunning(){
    return this->running;
}

void Game::setRunning(bool running){
    this->running = running;
}

void Game::run(){
  Uint64 count = 0;

  while(this->running){

    systemTimerEnd = std::chrono::high_resolution_clock::now();
    elapsedSeconds = systemTimerEnd - systemTimerStart;
    accumulatedSeconds += elapsedSeconds.count();
    systemTimerStart = systemTimerEnd;

    if(count % 16 == 0)
      DialogueSystem::Instance()->setFPS(1.0f/(elapsedSeconds).count());

    count++;

    if(accumulatedSeconds > cycleTime.count()){
      accumulatedSeconds -= cycleTime.count();  
      update();    
    }

    render();

  }

}

void Game::update(){
  this->gameStates.top()->update();
}

void Game::render(){
  this->gameStates.top()->render();
}

GAME_STATE Game::getGameState(){
  return this->gameState;
}

void Game::setGameState(GAME_STATE gameState){
  this->gameState = gameState;
}



void Game::test(){
  for(Uint16 i = 0; i < 1; i++){
    //EntityManager::Instance()->createEntity();
  }
  //SpriteSystem::Instance()->getSpriteComponentSystem()->printComponents();
}


void Game::pushGameState(GameState* gameState){
  this->gameStates.push(gameState);
}
void Game::popGameState(){
  this->gameStates.pop();
}