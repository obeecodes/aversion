#pragma once 

#include <stack>
#include "gameStates/gameState.h"
#include <SDL2/SDL_stdinc.h>

enum GAME_STATE{
    MAIN_MENU_STATE,
    TRANSITION_STATE,
    PLAY_STATE,
    PAUSE_STATE,
    GAME_OVER_STATE,
};

class Game{
public:
    static Game* Instance();
    bool getRunning();
    void setRunning(bool running);
    GAME_STATE getGameState();
    void setGameState(GAME_STATE gameState);
    void run();
    void test();

    void pushGameState(GameState* gameState);
    void popGameState();

    void update();
    void render();

    ~Game();

private:
    static Game* instance;
    bool running;
    GAME_STATE gameState;
    std::stack<GameState*> gameStates;
    Game();

    Uint64 fps;
    std::chrono::duration<float> cycleTime;
    std::chrono::high_resolution_clock::time_point systemTimerStart;
    std::chrono::high_resolution_clock::time_point systemTimerEnd;
    std::chrono::duration<float> elapsedSeconds;
    double accumulatedSeconds;
};