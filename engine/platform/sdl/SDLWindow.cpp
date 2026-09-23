#include "SDLWindow.h"
#include "diagnostics/Log.h"
#include "diagnostics/Assert.h"
#include <glad/glad.h>
#include <iostream>
#include <format>

SDLWindow::SDLWindow() : m_Window(nullptr), m_Context(nullptr){}

SDLWindow::~SDLWindow(){
  Destroy();
}

/**
 * @brief Creates the SDL window and OpenGL context.
 *
 * It sets OpenGL attributes (currently version 4.6 Core Profile),
 * then creates the SDL window with the OpenGL flag,
 * OpenGL context, initializes GLAD,
 * sets swap interval (vsync) and log driver information
 * then sets the viewport to match the window size.
 *
 * @param title Window title.
 * @param width Window width in pixels.
 * @param height Window height in pixels.
 * @param fullscreen Whether to start in fullscreen mode.
 * @return true if all steps succeeded, false otherwise.
 */
bool SDLWindow::Create(const char* title, int width, int height, bool fullscreen){
  //Set OpenGL Core Profile version 4.6
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  //Get primary display resolution
  SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
  const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(displayID);

  int displayWidth = displayMode->w;
  int displayHeight = displayMode->h;

  //Use display resolution if invalid dimensions provided
  if(width <= 0 || height <= 0){
    width = displayWidth;
    height = displayHeight;
  }

  //Create SDL window with OpenGL support and resizable flag
  m_Window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  Log::Info(std::format("Window ID: {}", SDL_GetWindowID(m_Window)));
  Log::Info("Window successfully created.");

  //Validate window creation
  if(!m_Window){
    Log::Error(std::format("Window creation error: {}", SDL_GetError()));
    return false;
  }

  //Set fullscreen mode if requested
  if(fullscreen){
    if(SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN) != 0){
      Log::Warning(std::format("Failed to set fullscreen: {}", SDL_GetError()));
    } else{
      Log::Info("Fullscreen mode enabled.");
    }
  } else{
    //Center window on screen
    SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  }

  //Create OpenGL context
  m_Context = SDL_GL_CreateContext(m_Window);
  if(!m_Context){
    Log::Error(std::format("OpenGL context not created: {}", SDL_GetError()));
    Destroy();
    return false;
  }

  //Initialize GLAD (OpenGL function loader)
  if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
    Log::Error("GLAD initialization error.");
    Destroy();
    return false;
  }

  //Enable vsync (1 = on, 0 = off)
  SDL_GL_SetSwapInterval(1);

  //Log OpenGL information
  Log::Info(std::format("OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION))));
  Log::Info(std::format("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER))));

  //Set viewport to match window size
  int realWidth, realHeight;
  SDL_GetWindowSize(m_Window, &realWidth, &realHeight);
  glViewport(0, 0, realWidth, realHeight);

  return true;
}

/**
 * @brief Toggles between windowed and fullscreen mode.
 */
bool SDLWindow::ToggleFullscreen(){
  if(!m_Window) return false;

  bool isFullscreen = (SDL_GetWindowFlags(m_Window) & SDL_WINDOW_FULLSCREEN) != 0;
  bool newState = !isFullscreen;

  if(!SDL_SetWindowFullscreen(m_Window, newState)){
    Log::Warning(std::format("Failed to toggle fullscreen: {}", SDL_GetError()));
    return isFullscreen; //unchanged
  }

  Log::Info(newState ? "Fullscreen enabled." : "Windowed mode enabled.");
  return newState;
}

/**
 * @brief Checks if the window is currently fullscreen.
 */
bool SDLWindow::IsFullscreen() const{
  if(!m_Window) return false;
  return (SDL_GetWindowFlags(m_Window) & SDL_WINDOW_FULLSCREEN) != 0;
}


/**
 * @brief Destroys the OpenGL context and the SDL window.
 *
 * The context is destroyed first, then the window. Both pointers are set to nullptr
 * to prevent double deletion.
 */
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

/**
 * @brief Swaps the front and back buffers.
 *
 * This presents the rendered frame to the screen.
 */
void SDLWindow::SwapBuffers(){
  SDL_GL_SwapWindow(m_Window);
}

/**
 * @brief Gets the native SDL_Window handle.
 *
 * Used by other systems (e.g., Input, Engine) that need direct access to the window.
 *
 * @return Pointer to the SDL_Window.
 */
SDL_Window* SDLWindow::GetNativeWindow() const{
  return m_Window;
}
