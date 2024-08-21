#pragma once

#include <SDL2/SDL_render.h>

#define WINDOW_WIDTH 512//384//1470//640//1024//640
#define WINDOW_HEIGHT 288//216//920//360//576//360

class Window{
public:
    static Window* Instance();

    bool initWindow();
    void clean();

    //setters and getters
    void setTitle(const char* title);
    const char* getTitle() const;

    void setActive(bool active);
    bool getActive() const;

    void setWindow(SDL_Window* const window);
    SDL_Window* getWindow() const;

private:
    Window();

    static Window* instance;
    const char* title;
    bool active;

    SDL_Window* window;
};