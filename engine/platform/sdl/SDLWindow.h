#pragma once

#include <SDL3/SDL.h>

class SDLWindow{
  public:
    SDLWindow();
    ~SDLWindow();

    bool Create(const char* title, int width, int height);

    void Destroy();

    void SwapBuffers();

    SDL_Window* GetNativeWindow() const;

  private:
    SDL_Window* m_Window;
    SDL_GLContext m_Context;
};
