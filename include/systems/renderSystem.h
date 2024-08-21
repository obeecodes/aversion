#pragma once

class RenderSystem{
public:
  static RenderSystem* Instance();
  void update();
private:
  static RenderSystem* instance;
  RenderSystem();

};
