#pragma once

#include <SDL3/SDL.h>

class SDLWindow{
  public:
    SDLWindow();
    ~SDLWindow();

    bool Create(cont char* title, int width, int weigth);

    void SwapBuffers();

    SDL_Window* GetNativeWindow() const;

  private:
    SDL_Window* m_Window;
    SDL_GLContext m_Context;
};
