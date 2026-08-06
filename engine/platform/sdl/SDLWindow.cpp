#include "SDLWindow.h"
#include <glad/glad.h>
#include <iostream>

SDLWindow::SDLWindow() : m_Window(nullptr), m_Context(nullptr) {}

SDLWindow::~SDLWindow(){
  Destroy();
}

bool SDLWindow::Create(const char* title, int width, int height){
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  m_Window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);
  std::cout << "Window ID: " << SDL_GetWindowID(m_Window) << '\n'; 
  std::cout << "Window successfully created.\n";
  

  if (!m_Window){
    std::cerr << "Window creation error: " << SDL_GetError() << '\n';
    return false;
  }

  m_Context = SDL_GL_CreateContext(m_Window);

  if(!m_Context){
    std::cerr << "OpenGL context not created: " << SDL_GetError() << '\n';
    Destroy();
    return false;
  }

  if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
    std::cerr << "GLAD initialization error.\n";
    Destroy();
    return false;
  }

  SDL_GL_SetSwapInterval(1);
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';

  glViewport(0,0,width,height);
  return true;
  }

void SDLWindow::Destroy(){
  if(m_Context){
    SDL_GL_DestroyContext(m_Context);
    m_Context = nullptr;
  }

  if(m_Window){
    SDL_DestroyWindow(m_Window);
    m_Window = nullptr;
  }
}

void SDLWindow::SwapBuffers(){
  SDL_GL_SwapWindow(m_Window);
}
SDL_Window* SDLWindow::GetNativeWindow() const{
  return m_Window;
}
