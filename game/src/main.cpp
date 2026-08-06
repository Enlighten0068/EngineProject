#include <SDL3/SDL.h>
#include "SDLWindow.h"
#include <iostream>
#include <glad/glad.h>


bool Initialize(){
      std::cout << "SDL3 Runtime initialization.\n";

      if (SDL_Init(SDL_INIT_VIDEO) < 0)
      {
            std::cerr << "Initializing failed: " << SDL_GetError() << '\n';
            return false;
      }

      const char* videoDriver = SDL_GetCurrentVideoDriver();

      if (videoDriver) std::cout << "Video Driver: " << videoDriver << '\n';

      return true;
}

void Shutdown(){
      SDL_Quit();
      std::cout << "SDL3 terminated.\n";
}

void Run(SDLWindow& window){
      bool running = true;

      while (running){
            SDL_Event event;

            while (SDL_PollEvent(&event)){
                  if (event.type == SDL_EVENT_QUIT){
                        std::cout
                        << "Quit event received.\n";
                        running = false;
                  }
            }

            glClear(GL_COLOR_BUFFER_BIT);
            window.SwapBuffers();
      }
}

int main(){
      if (!Initialize()) return 1;

      SDLWindow window;

      if (!window.Create("Game Window",1920,1080)){
            Shutdown();
            return 1;
      }

      glClearColor(0.1f,0.2f,0.4f,1.0f);
      Run(window);
      Shutdown();
      return 0;
}
