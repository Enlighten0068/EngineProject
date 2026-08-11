#include "SDLWindow.h"
#include "diagnostics/Log.h"
#include <glad/glad.h>
#include <iostream>
#include <format>

SDLWindow::SDLWindow() : m_Window(nullptr), m_Context(nullptr) {}

SDLWindow::~SDLWindow(){
  Destroy();
}

bool SDLWindow::Create(const char* title, int width, int height){
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  m_Window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);
  Log::Info(std::format("Window ID: {}", SDL_GetWindowID(m_Window)));
  Log::Info("Window successfully created.");
  

  if (!m_Window){
    Log::Error(std::format("Window creation error: {}", SDL_GetError()));
    return false;
  }

  m_Context = SDL_GL_CreateContext(m_Window);

  if(!m_Context){
    Log::Error(std::format("OpenGL context not created: {}", SDL_GetError()));
    Destroy();
    return false;
  }

  if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
    Log::Error("GLAD initialization error.");
    Destroy();
    return false;
  }

  SDL_GL_SetSwapInterval(1);
  Log::Info(std::format("OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)))); //glGetString devolve GLubyte* - conversão para const char
  Log::Info(std::format("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)))); //glGetString devolve GLubyte* - conversão para const char

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
