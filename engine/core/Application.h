#pragma once

#include "core/Engine.h"
#include "core/PlayerController.h"
#include "core/FpsCounter.h"
#include "core/CameraController.h"
#include "core/GamepadManager.h"
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
#include "scene/ECSScene.h"
#include "scene/SceneManager.h"
#include "scene/GameWorld.h"
#include <entt.hpp>
#include <memory>


class Application{
public:
    Application();
    ~Application();

    bool Initialize();
    void Run();
    void Shutdown();

    SceneManager& GetSceneManager() { return m_SceneManager; }
    GraphicsContext& GetGraphicsContext() { return *m_Graphics; }

private:
    Engine m_Engine;
    std::unique_ptr<GraphicsContext> m_Graphics;
    std::unique_ptr<ECSScene> m_Scene;
    std::unique_ptr<Camera2D> m_Camera;
    std::unique_ptr<PlayerController> m_PlayerController;
    std::unique_ptr<FpsCounter> m_FpsCounter;
    std::unique_ptr<GameWorld> m_World;

    entt::entity m_PlayerEntity;
    SceneManager m_SceneManager;

    void ProcessEvents();
    void Render();
    void Update();
    void SetupControllers();
};
