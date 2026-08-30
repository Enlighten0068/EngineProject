#pragma once

#include "core/Engine.h"
#include "core/PlayerController.h"
#include "core/FpsCounter.h"
#include "core/CameraController.h"
#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"
#include "events/EventBus.h"
#include "graphics/Shader.h"
#include "graphics/IndexBuffer.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/Texture2D.h"
#include "graphics/GraphicsContext.h"
#include "resources/ResourceManager.h"
#include "scene/Camera2D.h"
#include "scene/Scene.h"
#include <entt.hpp>
#include <memory>


class Application{
public:
    Application();
    ~Application();

    bool Initialize();
    void Run();
    void Shutdown();

private:
    Engine m_Engine;
    std::unique_ptr<GraphicsContext> m_Graphics;
    std::unique_ptr<Scene> m_Scene;
    std::unique_ptr<Camera2D> m_Camera;
    std::unique_ptr<CameraController> m_CameraController;
    std::unique_ptr<PlayerController> m_PlayerController;
    std::unique_ptr<FpsCounter> m_FpsCounter;

    entt::entity m_PlayerEntity;

    void ProcessEvents();
    void Render();
    void Update();
    void SetupControllers();
};
