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

/**
 * @brief Main application class that orchestrates the entire game engine.
 *
 * The Application class is the central hub of the engine. It initializes all subsystems
 * (graphics, audio, input, gamepad, ECS, scene management) needed then runs the main game loop,
 * and handles shutdown. It delegates rendering and logic updates to the SceneManager
 * and other systems.
 *
 * @note Even though it's not a Singleton class, keep no more than one instance active.
 * @see Engine, GraphicsContext, SceneManager, PlayerController
 */
class Application{
public:
    Application();
    ~Application();

    /**
     * @brief Initializes all engine subsystems.
     * @return true if initialization succeeded, false otherwise.
     */
    bool Initialize();

    /**
     * @brief Runs the main game loop until the engine is stopped.
     *
     * The loop processes events, updates all systems, and renders the current scene.
     */
    void Run();

    /**
     * @brief Shuts down the application and releases all resources.
     *
     * This method ensures that all subsystems (graphics, audio, input, etc.)
     * are properly cleaned up before exiting the application.
     */
    void Shutdown();

    /**
     * @brief Get the SceneManager instance.
     * @return Reference to the SceneManager.
     */
    SceneManager& GetSceneManager(){ return m_SceneManager; }

    /**
     * @brief Get the GraphicsContext instance.
     * @return Reference to the GraphicsContext.
     */
    GraphicsContext& GetGraphicsContext(){ return *m_Graphics; }

private:
    Engine m_Engine; //Window, SDL
    std::unique_ptr<GraphicsContext> m_Graphics; //Shaders, OpenGL, Buffers
    std::unique_ptr<ECSScene> m_Scene; //Scene containing entities and components
    std::unique_ptr<Camera2D> m_Camera;
    std::unique_ptr<PlayerController> m_PlayerController; //Player entity actions
    std::unique_ptr<FpsCounter> m_FpsCounter;
    std::unique_ptr<GameWorld> m_World; //Game word - boundaries, colision resolution

    entt::entity m_PlayerEntity;
    SceneManager m_SceneManager;

    void ProcessEvents(); //Handles polls and SDL events
    void Render(); //Scene renderization
    void Update(); //Systems update during the current frame
    void SetupControllers(); //Player and camera controllers initialization
};
