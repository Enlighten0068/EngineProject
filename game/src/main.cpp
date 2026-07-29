#include <SDL3/SDL.h>
#include <iostream>


int main(){
  std::cout << "Teste de inicialização do SDL3.\n";
  if (SDL_Init(SDL_INIT_VIDEO) < 0 ){
    std::cout << "Erro na inicialização: " << SDL_GetError() << '\n';
    return 1;
  }
  else{
    std::cout << "Sucesso.\n";
  }
  SDL_Quit();
  return 0;
}
