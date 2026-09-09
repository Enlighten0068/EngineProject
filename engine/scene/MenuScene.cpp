#include "core/Input.h"
#include "core/GamepadManager.h"
#include "demo/Demo2DFixedScene.h"
#include "diagnostics/Log.h"
#include "graphics/Texture2D.h"
#include "renderer/Renderer.h"
#include "scene/MenuScene.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <format>

MenuScene::MenuScene(SceneManager& sceneManager, Shader& shader, VertexArray& va, IndexBuffer& ib,
                     Camera2D& camera, Shader& lineShader)
: m_SceneManager(sceneManager), m_Shader(shader), m_VertexArray(va)
, m_IndexBuffer(ib), m_Camera(camera), m_LineShader(lineShader){}

MenuScene::~MenuScene(){
    OnExit();
}

/**
 * @brief Called when the menu becomes active.
 *
 * Loads the font, creates the menu options, and sets up the background.
 */
void MenuScene::OnEnter(){
    Log::Info("Entering MenuScene");

    //Font loading
    m_Font = std::make_unique<Font>();
    if (!m_Font->Load("assets/fonts/arial.ttf", 96)) Log::Error("Failed to load font. Menu will not display text.");


    //Create menu options
    m_Options.clear();

    auto createOption = [this](const std::string& label, float yPos){
        SDL_Color color = m_NormalColor;
        auto texture = CreateTextTexture(label, color);
        if (texture){
            MenuOption option;
            option.Label = label;
            option.Texture = texture;
            option.Position = Vector3D(0.0f, yPos, 0.0f);
            option.Width = static_cast<float>(texture->GetWidth());
            option.Height = static_cast<float>(texture->GetHeight());
            m_Options.push_back(option);
        }
    };

    //Position options vertically with spacing
    float startY = 2.0f;
    float spacing = 2.5f;
    createOption("Play", startY + spacing * 2);
    createOption("Options", startY + spacing);
    createOption("Exit", startY);

    //Load Background texture
    //m_BackgroundTexture = ResourceManager::GetInstance().LoadTexture("assets/textures/menu_bg.png");

    m_SelectedOption = 0;
}

/**
 * @brief Called when the menu is exited.
 *
 * Releases resources used by the menu.
 */
void MenuScene::OnExit(){
    m_Options.clear();
    m_Font.reset();
    m_BackgroundTexture.reset();
    Log::Info("Exiting MenuScene");
}

/**
 * @brief Creates a texture from rendered text.
 *
 * Renders the text using SDL_ttf, converts the surface to an OpenGL texture,
 * and returns it as a shared pointer.
 *
 * @param text The string to render.
 * @param color The color of the text.
 * @return Shared pointer to the Texture2D, or nullptr on failure.
 */
std::shared_ptr<Texture2D> MenuScene::CreateTextTexture(const std::string& text, const SDL_Color& color){
    if (!m_Font || !m_Font->IsLoaded()){
        Log::Warning("Cannot render text: no font loaded.");
        return nullptr;
    }

    SDL_Surface* surface = m_Font->RenderText(text, color);
    if (!surface){
        Log::Error("RenderText returned null surface for: " + text);
        return nullptr;
    }

    auto texture = std::make_shared<Texture2D>();
    if (!texture->LoadFromSurface(surface)){
        Log::Error("LoadFromSurface failed for: " + text);
        SDL_DestroySurface(surface);
        return nullptr;
    }

    glBindTexture(GL_TEXTURE_2D, texture->GetRendererID());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    SDL_DestroySurface(surface);
    Log::Info("Text texture created: " + text);
    return texture;
}

/**
 * @brief Updates the menu: navigation and selection.
 *
 * Arrow keys or WASD navigate the menu, Enter / Space / A confirms selection.
 *
 * @param deltaTime Time elapsed since the last frame.
 */
void MenuScene::Update(float deltaTime){
    //Navigation - Down
    if (Input::IsKeyPressed(SDL_SCANCODE_DOWN) || Input::IsKeyPressed(SDL_SCANCODE_S) ||
        GamepadManager::GetInstance().IsButtonPressed(0, SDL_GAMEPAD_BUTTON_DPAD_DOWN)){
        m_SelectedOption = (m_SelectedOption + 1) % static_cast<int>(m_Options.size());
        Log::Info("Selected: " + m_Options[m_SelectedOption].Label);
    }

    //Navigation - Up
    if (Input::IsKeyPressed(SDL_SCANCODE_UP) || Input::IsKeyPressed(SDL_SCANCODE_W) ||
        GamepadManager::GetInstance().IsButtonPressed(0, SDL_GAMEPAD_BUTTON_DPAD_UP)){
        m_SelectedOption = (m_SelectedOption - 1 + static_cast<int>(m_Options.size())) % static_cast<int>(m_Options.size());
        Log::Info("Selected: " + m_Options[m_SelectedOption].Label);
    }

    //Navigation - Confirm
    if (Input::IsKeyPressed(SDL_SCANCODE_RETURN) || Input::IsKeyPressed(SDL_SCANCODE_SPACE) ||
        GamepadManager::GetInstance().IsButtonPressed(0, SDL_GAMEPAD_BUTTON_EAST)){
        HandleSelection();
    }

    //Mouse button
    if (Input::IsMouseButtonPressed(1)){
        Vector2D mousePos = Input::GetMousePosition();
        int winWidth = Input::GetWindowWidth();
        int winHeight = Input::GetWindowHeight();

        //Convert to world coordinates
        float worldX = (mousePos.x / winWidth) * 20.0f - 10.0f;
        float worldY = -(mousePos.y / winHeight) * 20.0f + 10.0f; // inverter Y

        for (size_t i = 0; i < m_Options.size(); ++i){
            const auto& opt = m_Options[i];
            float halfWidth = opt.Width * 0.5f * (1.5f / opt.Height) * 1.5f;
            float halfHeight = 0.75f;

            float left = opt.Position.x - halfWidth;
            float right = opt.Position.x + halfWidth;
            float bottom = opt.Position.y - halfHeight;
            float top = opt.Position.y + halfHeight;

            if (worldX >= left && worldX <= right && worldY >= bottom && worldY <= top){
                m_SelectedOption = static_cast<int>(i);
                HandleSelection();
                break;
            }
        }
    }
}

/**
 * @brief Renders the menu.
 *
 * Clears the screen, draws the background (if any), and renders all menu options.
 */
void MenuScene::Render(){
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Render menu options
    for (size_t i = 0; i < m_Options.size(); ++i){
        bool isSelected = (static_cast<int>(i) == m_SelectedOption);
        RenderOption(m_Options[i], isSelected);
    }
}

/**
 * @brief Renders a single menu option.
 * @param option The menu option to render.
 * @param isSelected Whether this option is currently selected.
 */
void MenuScene::RenderOption(const MenuOption& option, bool isSelected){
    if (!option.Texture){
        Log::Warning("RenderOption: texture is null for " + option.Label);
        return;
    }
    Log::Info("Rendering option: " + option.Label);

    //Build model matrix
    Matrix4 model = Matrix4::Translation(option.Position);

    float aspect = option.Width / option.Height;
    float height = 1.5f;
    float width = height * aspect;
    model = model * Matrix4::Scale(Vector3D(width, height, 1.0f));

    //Get view and projection
    Matrix4 view = Matrix4::Identity();
    Matrix4 projection = Matrix4::Orthographic(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);

    //Render the text texture
    Renderer::DrawTexturedQuad(m_Shader, m_VertexArray, m_IndexBuffer,
                               *option.Texture, model, view, projection, 1.0f, 1.0f);
}

/**
 * @brief Handles the selection of the current menu option.
 */
void MenuScene::HandleSelection(){
    if (m_Options.empty()) return;

    const std::string& selected = m_Options[m_SelectedOption].Label;

    if (selected == "Play"){
        Log::Info("Starting Demo 2D...");
        //Transition to the demo scene
        auto scene = std::make_unique<Demo2DFixedScene>(m_Shader, m_VertexArray,
                                                        m_IndexBuffer, m_Camera, m_LineShader);
        m_SceneManager.SetScene(std::move(scene));
    } else if (selected == "Options"){
        Log::Info("Options menu to be implemented later.");
    } else if (selected == "Exit"){
        Log::Info("Exiting game...");
        SDL_Event quitEvent{ .type = SDL_EVENT_QUIT };
        SDL_PushEvent(&quitEvent);
    }
}
