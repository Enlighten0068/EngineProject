#include "core/Application.h"
#include "demo/Demo2DScene.h"
#include "demo/Demo2DFixedScene.h"
#include "scene/MenuScene.h"
#include <memory>
#include <iostream>

/**
 * @brief Entry point for the EngineProject application.
 *
 * Creates the main Application instance, initializes all engine subsystems,
 * and selects the initial scene based on the DEMO_2D_FIXED preprocessor macro.
 *
 * If DEMO_2D_FIXED is defined, the application starts directly in the
 * Demo2DFixedScene (a 2D platformer demo with a fixed camera).
 * Otherwise, it starts in the MenuScene, allowing the user to navigate
 * to different game modes.
 *
 * @return int Returns 0 on successful execution, 1 if initialization fails.
 *
 * @see Application, Demo2DFixedScene, MenuScene
 */
int main(){
      //Create the main application instance
      Application app;

      //Initialize all engine subsystems (graphics, audio, input, etc.)
      if(!app.Initialize()) return 1;

      //Obtain a reference to the graphics context for shader and buffer access
      auto& graphics = app.GetGraphicsContext();

      #ifdef DEMO_2D_FIXED
      //Directly load the demo scene (bypasses the main menu)
      auto scene = std::make_unique<Demo2DFixedScene>(graphics.GetShader(), graphics.GetVertexArray(),
                                                      graphics.GetIndexBuffer(),
                                                      app.GetCamera(), graphics.GetLineShader(),);
      #else
      //Load the main menu scene (default behavior)
      auto scene = std::make_unique<MenuScene>(app.GetSceneManager(), graphics.GetShader(),
                                               graphics.GetVertexArray(), graphics.GetIndexBuffer(),
                                               app.GetCamera(), graphics.GetLineShader());
      #endif
      //Set the active scene in the scene manager
      app.GetSceneManager().SetScene(std::move(scene));

      //Run the main game loop
      app.Run();

      //Clean up all resources before exiting
      app.Shutdown();

      return 0;
}
