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
      auto scene = std::make_unique<Demo2DFixedScene>(graphics.GetShader(), graphics.GetVertexArray(), graphics.GetIndexBuffer());
#elif defined(MENU)
      auto scene = std::make_unique<MenuScene>(app.GetSceneManager());
#else
      auto scene = std::make_unique<Demo2DScene>(graphics.GetShader(), graphics.GetVertexArray(), graphics.GetIndexBuffer());
#endif

      app.GetSceneManager().SetScene(std::move(scene));
      app.Run();
      app.Shutdown();
      return 0;
}
