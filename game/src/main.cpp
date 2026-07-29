#include <SDL3/SDL.h>
#include <iostream>


int main(){
  std::cout << "SDL3 Runtime initialization.\n";
  if (SDL_Init(SDL_INIT_VIDEO) == -1){
    std::cerr << "Initializing failed: " << SDL_GetError() << '\n';
    return 1;
  }

  //std::cout << "Sucess.\n";
  
  const char* videoDriver = SDL_GetCurrentVideoDriver();

  if (videoDriver) std::cout << "Video Driver: " << videoDriver << '\n';

  SDL_Window* window = SDL_CreateWindow("Game Window", 1920, 1080, SDL_WINDOW_RESIZABLE);

  if (!window){
    std::cerr << "Window creation error: " << SDL_GetError() << '\n';
    SDL_Quit();
    return 1;
  }
  std::cout << "Window ID: " << SDL_GetWindowID(window) << '\n';
  std::cout << "Window successfully created.\n";
  
  SDL_Delay(10000);
  SDL_DestroyWindow(window);
  std::cout << "Window disposal success.\n";

  SDL_Quit();
  std::cout << "SDL3 terminated.\n";

  return 0;
}
