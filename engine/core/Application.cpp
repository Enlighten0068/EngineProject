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
    //Initialize logging system
    Log::Initialize("logs/");
    Log::Info("Application: Initializing...");

    //Initialize SDL with video and audio subsystems
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
        Log::Error(std::format("SDL Init failed: {}", SDL_GetError()));
        return false;
    }

    //Initialize SDL_ttf for font rendering
    if(TTF_Init() != 0){
        Log::Error(std::format("SDL_ttf initialization failed: {}", SDL_GetError()));
    } else{
        Log::Info("SDL_ttf initialized successfully.");
    }

    //Create the game window
    if(!m_Engine.Initialize("Game Window", 1920, 1080, false)){
        Log::Error("Failed to initialize Engine.");
        return false;
    }

    //Subscribe to window close event
    EventBus::GetInstance().Subscribe<WindowCloseEvent>([this](Event& e){
        Log::Info("WindowCloseEvent received, shutting down game.");
        m_Engine.SetRunning(false);
    });

    //Subscribe to window resize event
    EventBus::GetInstance().Subscribe<WindowResizeEvent>([](Event& e){
        WindowResizeEvent& resize = static_cast<WindowResizeEvent&>(e);
        Log::Info(std::format("Window resized to {}x{}", resize.GetWidth(), resize.GetHeight()));
    });

    //Set callback for unhandled SDL events
    SDLEventTranslator::SetUnhandledCallback([](const SDL_Event& event){
        Log::Warning(std::format("Unhandled SDL event type: {}", event.type));
    });

    //Initialize OpenGL graphics context
    m_Graphics = std::make_unique<GraphicsContext>();
    if(!m_Graphics->Initialize()){
        Log::Error("GraphicsContext initialization failed.");
        return false;
    }

    //Initialize HUD overlay
    m_HUD = std::make_unique<HUD>();
    m_HUD->Initialize("assets/fonts/arial.ttf", 28);

    //Initialize camera with aspect ratio
    int winWidth, winHeight;
    SDL_GetWindowSize(m_Engine.GetWindow().GetNativeWindow(), &winWidth, &winHeight);
    Input::SetWindowSize(winWidth, winHeight);
    float aspect = static_cast<float>(winWidth) / static_cast<float>(winHeight);
    float height = 5.0f;
    float width = height * aspect;
    m_Camera = std::make_unique<Camera2D>(-width, width, -height, height);
    m_Camera->SetPosition(Vector3D(0.0f, 0.0f, 0.0f));

    //Initialize game world boundaries
    m_World = std::make_unique<GameWorld>(-20.0f, 20.0f, -10.0f, 10.0f);

    //Initialize gamepad manager
    if(!GamepadManager::GetInstance().Initialize()){
        Log::Warning("Gamepad initialization failed. Continuing without gamepad support.");
    }

    //Initialize FPS counter
    m_FpsCounter = std::make_unique<FpsCounter>();

    //Subscribe to window resize events — delegate to the current scene
    EventBus::GetInstance().Subscribe<WindowResizeEvent>([this](Event& e){
        WindowResizeEvent& resizeEvent = static_cast<WindowResizeEvent&>(e);
        int newWidth = resizeEvent.GetWidth();
        int newHeight = resizeEvent.GetHeight();

        Input::SetWindowSize(newWidth, newHeight);

        //Let the current scene decide how to handle the resize
        Scene* currentScene = m_SceneManager.GetCurrentScene();
        if(currentScene){
            currentScene->OnResize(newWidth, newHeight);
        }
    });

    Log::Info("Application initialized successfully.");

    //Initial scene - Demo2DFixedScene
    auto initialScene = std::make_unique<Demo2DFixedScene>(
        m_Graphics->GetShader(), m_Graphics->GetVertexArray(),
        m_Graphics->GetIndexBuffer(), *m_Camera, m_Graphics->GetLineShader());

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
    while(m_Engine.IsRunning()){
        Time::Update(); //Update time system

        //Clamp delta time to prevent massive jumps after a long freeze
        //(e.g., while the user drags the window border)
        constexpr float MAX_DELTA = 0.05f; //50 ms = min 20 FPS
        if(Time::DeltaTime() > MAX_DELTA) Time::SetDeltaTime(MAX_DELTA);

        ProcessEvents(); //Process SDL events
        Update(); //Update game logic
        Input::Update(); //Reset input deltas
        Render(); //Render the scene
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
    ResourceManager::GetInstance().Clear();
    m_World.reset();
    m_Camera.reset();

    if(m_HUD){
        m_HUD->Shutdown();
        m_HUD.reset();
    }

    if(m_Graphics){
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
    //Global hotkey: F11 = toggle fullscreen
    if(Input::IsKeyPressed(SDL_SCANCODE_F11)) m_Engine.ToggleFullscreen();

    GamepadManager::GetInstance().Update();
    m_SceneManager.Update(Time::DeltaTime());
}

/**
 * @brief Renders the current scene.
 *
 * Clears buffers and delegates rendering to the Scene Manager.
 */
void Application::Render(){
    int winWidth = Input::GetWindowWidth();
    int winHeight = Input::GetWindowHeight();
    glViewport(0, 0, winWidth, winHeight);

    glClear(GL_COLOR_BUFFER_BIT);
    m_SceneManager.Render();

    //HUD overlay (renders on top of the scene)
    if(m_HUD){
        Scene* currentScene = m_SceneManager.GetCurrentScene();
        if(currentScene){
            m_HUD->Render(currentScene->GetHUDLines(),
            m_Graphics->GetShader(),
            m_Graphics->GetVertexArray(),
            m_Graphics->GetIndexBuffer(),
            winWidth, winHeight);
        }
    }

    m_Engine.GetWindow().SwapBuffers();
}
