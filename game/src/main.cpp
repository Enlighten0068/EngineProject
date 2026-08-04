#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>


int main(){
  std::cout << "SDL3 Runtime initialization.\n";
  if (SDL_Init(SDL_INIT_VIDEO) == -1){
    std::cerr << "Initializing failed: " << SDL_GetError() << '\n';
    return 1;
  }
  
  const char* videoDriver = SDL_GetCurrentVideoDriver();

  if (videoDriver) std::cout << "Video Driver: " << videoDriver << '\n';

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_Window* window = SDL_CreateWindow("Game Window", 1920, 1080, SDL_WINDOW_OPENGL);
  if (!window){
    std::cerr << "Window creation error: " << SDL_GetError() << '\n';
    SDL_Quit();
    return 1;
  }

  SDL_GLContext glContext = SDL_GL_CreateContext(window);
  if(!glContext){
    std::cerr << "OpenGL context not created: " << SDL_GetError() << '\n';
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  SDL_GL_SetSwapInterval(1);

  //GLAD initialization
  if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
    std::cerr << "GLAD initialization error.\n";
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
  std::cout << "Window ID: " << SDL_GetWindowID(window) << '\n';
  std::cout << "Window successfully created.\n";

  //Background color 
  glClearColor(0.1f,0.2f,0.4f,1.0f);

  bool running = true;

  while(running){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_EVENT_QUIT) running = false;
    }

    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DestroyContext(glContext);
  std::cout << "OpenGL terminated.\n";

  SDL_DestroyWindow(window);
  std::cout << "Window disposal success.\n";

  SDL_Quit();
  std::cout << "SDL3 terminated.\n";

  return 0;
}
