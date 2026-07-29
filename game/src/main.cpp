#include <SDL3/SDL.h>
#include <iostream>


int main(){
  std::cout << "SDL3 Runtime initialization.\n";
  if (SDL_Init(SDL_INIT_VIDEO) < 0 ){
    std::cout << "Initializing failed: " << SDL_GetError() << '\n';
    return 1;
  }
  else{
    std::cout << "Sucess.\n";
  }
  SDL_Quit();
  return 0;
}
