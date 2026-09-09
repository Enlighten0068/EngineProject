#include "core/Application.h"
#include "demo/Demo2DScene.h"
#include "demo/Demo2DFixedScene.h"
#include "scene/MenuScene.h"
#include <memory>
#include <iostream>

int main(){
      Application app;

      if(!app.Initialize()) return 1;

      auto& graphics = app.GetGraphicsContext();

      #ifdef DEMO_2D_FIXED
      //Directly load the demo scene
      auto scene = std::make_unique<Demo2DFixedScene>(graphics.GetShader(), graphics.GetVertexArray(),
                                                      graphics.GetIndexBuffer(),
                                                      app.GetCamera(), graphics.GetLineShader(),);
      #else
      //Load the menu scene
      auto scene = std::make_unique<MenuScene>(app.GetSceneManager(), graphics.GetShader(),
                                               graphics.GetVertexArray(), graphics.GetIndexBuffer(),
                                               app.GetCamera(), graphics.GetLineShader());

      #endif

      app.GetSceneManager().SetScene(std::move(scene));
      app.Run();
      app.Shutdown();
      return 0;
}
