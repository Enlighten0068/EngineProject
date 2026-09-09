#pragma once

#include <SDL3/SDL.h>

/**
 * @brief Wrapper for an SDL3 window with OpenGL context.
 *
 * This class manages the creation, destruction, and swapping of an SDL window
 * with an OpenGL context. It abstracts the platform-specific window handling
 * and provides a clean interface for the engine.
 *
 * @note The OpenGL context is created with version 4.6 Core Profile.
 * @see Engine, GraphicsContext
 */
class SDLWindow{
public:
  SDLWindow();
  ~SDLWindow();

  /**
   * @brief Creates an SDL window with an OpenGL context.
   *
   * This method sets OpenGL attributes (version 4.6 Core Profile),
   * creates the window, initializes GLAD (OpenGL function loader),
   * and configures the viewport.
   *
   * @param title Window title.
   * @param width Window width in pixels.
   * @param height Window height in pixels.
   * @param fullscreen Whether to start in fullscreen mode.
   * @return true if the window and context were created successfully, false otherwise.
   */
  bool Create(const char* title, int width, int height, bool fullscreen);

  /**
   * @brief Destroys the window and OpenGL context.
   */
  void Destroy();

  /**
   * @brief Swaps the front and back buffers (presents the rendered frame).
   */
  void SwapBuffers();

  /**
   * @brief Gets the native SDL_Window handle.
   * @return Pointer to the SDL_Window.
   */
  SDL_Window* GetNativeWindow() const;

private:
  SDL_Window* m_Window; //SDL window handle
  SDL_GLContext m_Context; //OpenGL context handle
};
