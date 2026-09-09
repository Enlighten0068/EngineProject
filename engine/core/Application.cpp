#define GLAD_GL_IMPLEMENTATION

#include "audio/SoundManager.h"
#include "audio/SoundEffect.h"
#include "components/Transform.h"
#include "components/SpriteRenderer.h"
#include "core/Application.h"
#include "core/Time.h"
#include "core/Input.h"
#include "core/Engine.h"
#include "core/PlayerController.h"
#include "core/CameraController.h"
#include "core/FpsCounter.h"
#include "demo/Demo2DScene.h"
#include "demo/Demo2DFixedScene.h"
#include "diagnostics/Log.h"
#include "events/EventType.h"
#include "events/EventBus.h"
#include "events/WindowEvent.h"
#include "events/SDLEventTranslator.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/Shader.h"
#include "graphics/Texture2D.h"
#include "math/Matrix4.h"
#include "renderer/Renderer.h"
#include "resources/ResourceManager.h"
#include "scene/Camera2D.h"
#include "scene/GameWorld.h"
#include "scene/MenuScene.h"
#include "systems/RenderSystem.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <format>
#include <cstdint>

Application::Application() = default;
Application::~Application(){ Shutdown(); }

/**
 * @brief Initializes all engine subsystems and sets up the initial scene.
 *
 * It must follow an initialization order to work correctly starting from logging,
 * audio, engine, SDL_TTF, event system, graphics context, camera/world, scene and controllers.
 *
 * @return true if all subsystems initialized successfully, false otherwise.
 */
bool Application::Initialize(){
    //Logging
    Log::Initialize("logs/");
    //For testing purposes - uncomment to validate logging initialization
    /*Log::Info("Logging...");
    Log::Warning("Warning test.");
    Log::Error("Error test.");*/
    Log::Info("Application: Initializing...");

    //Audio initialization
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
        Log::Error(std::format("SDL Init failed: {}", SDL_GetError()));
        return false;
    }

    //SDL_ttf initialization
    if (TTF_Init() != 0) {
        Log::Error(std::format("SDL_ttf initialization failed: {}", SDL_GetError()));
    } else{
        Log::Info("SDL_ttf initialized successfully.");
    }

    //Game window initialization (engine), change last parameter to true if fullscreen
    if (!m_Engine.Initialize("Game Window", 1920, 1080, false)){
        Log::Error("Failed to initialize Engine.");
        return false;
    }

    //Event Handling System initialization
    EventBus::GetInstance().Subscribe<WindowCloseEvent>([this](Event& e) {
        Log::Info("WindowCloseEvent received, shutting down game.");
        m_Engine.SetRunning(false);
    });

    EventBus::GetInstance().Subscribe<WindowResizeEvent>([](Event& e) {
        WindowResizeEvent& resize = static_cast<WindowResizeEvent&>(e);
        Log::Info(std::format("Window resized to {}x{}", resize.GetWidth(), resize.GetHeight()));
    });


    SDLEventTranslator::SetUnhandledCallback([](const SDL_Event& event) {
        Log::Warning(std::format("Unhandled SDL event type: {}", event.type));
    });

    //GraphicsContext initialization
    m_Graphics = std::make_unique<GraphicsContext>();
    if (!m_Graphics->Initialize()) {
        Log::Error("GraphicsContext initialization failed.");
        return false;
    }

    //Camera initialization
    int winWidth, winHeight;
    SDL_GetWindowSize(m_Engine.GetWindow().GetNativeWindow(), &winWidth, &winHeight);
    Input::SetWindowSize(winWidth, winHeight);
    float aspect = static_cast<float>(winWidth) / static_cast<float>(winHeight);
    float height = 5.0f;
    float width = height * aspect;
    m_Camera = std::make_unique<Camera2D>(-width, width, -height, height);
    m_Camera->SetPosition(Vector3D(0.0f, 0.0f, 0.0f));

    //World initialization
    m_World = std::make_unique<GameWorld>(-20.0f, 20.0f, -10.0f, 10.0f);

    //Scene initialization
    /*m_Scene = std::make_unique<ECSScene>(m_Graphics->GetShader(),
                                      m_Graphics->GetVertexArray(),
                                      m_Graphics->GetIndexBuffer());*/



    //Player Entity
    auto texture = ResourceManager::GetInstance().LoadTexture("assets/textures/test.png");
    if (!texture){
        Log::Error("Failed to load texture.");
        return false;
    }
    //m_PlayerEntity = m_Scene->CreateSpriteEntity(
    //    Vector3D(0.0f, 0.0f, 0.0f), Vector3D(5.0f, 5.0f, 1.0f),texture);

    //Controllers
    //SetupControllers();


    //Check if any gamepad is available
    if (!GamepadManager::GetInstance().Initialize()) {
        Log::Warning("Gamepad initialization failed. Continuing without gamepad support.");
    }

    //FPS Counter
    m_FpsCounter = std::make_unique<FpsCounter>();

    EventBus::GetInstance().Subscribe<WindowResizeEvent>([this](Event& e){
        WindowResizeEvent& resizeEvent = static_cast<WindowResizeEvent&>(e);
        int newWidth = resizeEvent.GetWidth();
        int newHeight = resizeEvent.GetHeight();

        Input::SetWindowSize(newWidth, newHeight);

        //Verifies if the scene is the fixed 2D Scene, skips camera update if true
        Scene* currentScene = m_SceneManager.GetCurrentScene();
        if (currentScene && currentScene->GetName() == "Demo2D_FixedCamera"){
            Log::Info("Fixed camera scene active: skipping camera projection update.");
            return;
        }

        if (m_Camera){
            float aspect = static_cast<float>(newWidth) / static_cast<float>(newHeight);
            float height = 5.0f;
            float width = height * aspect;
            m_Camera->SetProjection(-width, width, -height, height);
        }
    });

    Log::Info("Application initialized successfully.");

    //Initial scene
    auto initialScene = std::make_unique<Demo2DFixedScene>(m_Graphics->GetShader(),
                                                           m_Graphics->GetVertexArray(), m_Graphics->GetIndexBuffer(),
                                                           *m_Camera, m_Graphics->GetLineShader());

    //Initial Menu scene - to be implemented
    /*auto menuScene = std::make_unique<MenuScene>(m_SceneManager);
    m_SceneManager.SetScene(std::move(menuScene));*/

    return true;
}

/**
 * @brief Main game loop.
 *
 * The loop runs while the engine is running, processing events, updating
 * all systems, and rendering the current scene. It follows a strict order starting from
 * time then event handling, system updates, input state, rendering.
 */
void Application::Run(){
    while (m_Engine.IsRunning()){
        Time::Update();
        ProcessEvents();
        Update();
        Input::Update(); //Reset input deltas
        Render();
    }
}

/**
 * @brief Shuts down the application and releases all resources.
 *
 * The shutdown order is the reverse of initialization to ensure proper
 * resource cleanup and avoid dangling references.
 */
void Application::Shutdown(){
    Log::Info("Shutting down Application...");

    m_FpsCounter.reset();
    GamepadManager::GetInstance().Shutdown();

    //m_PlayerController.reset();
    ResourceManager::GetInstance().Clear();
    //m_Scene.reset();
    m_World.reset();
    m_Camera.reset();

    if (m_Graphics){
        m_Graphics->Shutdown();
        m_Graphics.reset();
    }

    EventBus::GetInstance().Clear();
    Log::Info("EventBus cleared.");

    m_Engine.Shutdown();
    Log::Info("Engine shut down.");

    TTF_Quit();
    Log::Info("SDL_ttf shut down.");

    SoundEffect::CloseAudioDevice();

    Log::Shutdown();
}

/**
 * @brief Processes all pending SDL events.
 *
 * Events are passed to Input (for state tracking), GamepadManager (for gamepad events),
 * and SDLEventTranslator (for converting to engine events and dispatching via EventBus).
 */
void Application::ProcessEvents(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        Input::ProcessEvent(event);
        GamepadManager::GetInstance().ProcessEvent(event);
        SDLEventTranslator::TranslateAndDispatch(event);
    }
}

/**
 * @brief Updates all systems for the current frame.
 *
 * This includes the gamepad state and the current scene (which updates
 * entities, physics, AI)
 */
void Application::Update(){
    GamepadManager::GetInstance().Update();
    m_SceneManager.Update(Time::DeltaTime());
}

/**
 * @brief Renders the current scene.
 *
 * Clears buffers and delegates rendering to the Scene Manager.
 */
void Application::Render(){
    //Update viewport to window size
    int winWidth = Input::GetWindowWidth();
    int winHeight = Input::GetWindowHeight();
    glViewport(0, 0, winWidth, winHeight);

    glClear(GL_COLOR_BUFFER_BIT); //Buffer clearing
    m_SceneManager.Render();
    m_Engine.GetWindow().SwapBuffers();
}

/**
 * @brief Initializes the player and camera controllers.
 *
 * Currently sets up the player controller with the world and entity.
 */
/*void Application::SetupControllers(){
    //Player controller
    m_PlayerController = std::make_unique<PlayerController>(m_Scene->GetRegistry(), m_PlayerEntity, *m_World);
    m_PlayerController->SetSpeed(3.0f);

    //For 2D demo with follow Camera
    m_CameraController = std::make_unique<CameraController>(*m_Camera);
    m_CameraController->SetSpeed(5.0f);
    m_CameraController->SetFollowEntity(true);
    m_CameraController->SetZoomSpeed(1.0f);

    Log::Info("Controllers initialized.");
}*/
