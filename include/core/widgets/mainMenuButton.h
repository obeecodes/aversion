#pragma once 
#include "button.h"

class MainMenuButton : public Button {
public:
    MainMenuButton(int x, int y, int w, int h, const std::string& text, std::function<void(void)> callback);
    void render() override;
    void update() override;

    static const int BUTTON_WIDTH = 80;
    static const int BUTTON_HEIGHT = 12;

private:
    
    int offset;
    std::string text;
    SDL_Rect textDest;
    SDL_Color textColor;

    SDL_Texture* textTexture;


    void initButton();
};
