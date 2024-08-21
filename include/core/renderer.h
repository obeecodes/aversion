#pragma once

#include <SDL2/SDL_render.h>

//class definition
class Renderer{
public:

  //core
  bool initRenderer();
  void render();
  void update();
  //void handleEvents();
  void clean();

  //setters and getters
  void setRunning(bool running);
  bool getRunning();

  void setRenderer(SDL_Renderer* renderer);
  SDL_Renderer* getRenderer();

  void setTexture(SDL_Texture* texture);
  SDL_Texture* getTexture();

  void setSourceRectangle(SDL_Rect sourceRectangle);
  SDL_Rect getSourceRectangle();

  void setDestinationRectangle(SDL_Rect sourceRectangle);
  SDL_Rect getDestinationRectangle();

  void renderClear();
  void renderPresent();

  bool isM1iMac();

  static Renderer* Instance();
  
private:
  Renderer();
  static Renderer* instance;
  bool running;
  SDL_Renderer* renderer;
   
};
