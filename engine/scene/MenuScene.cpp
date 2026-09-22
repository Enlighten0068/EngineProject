#include "core/Input.h"
#include "core/GamepadManager.h"
//#include "demo/Demo2DFixedScene.h"
#include "diagnostics/Log.h"
#include "graphics/Texture2D.h"
#include "level/LevelSelectorScene.h"
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

    //Load font
    m_Font = std::make_unique<Font>();
    if(!m_Font->Load("assets/fonts/arial.ttf", 96)){
        Log::Error("Failed to load font. Menu will not display text.");
    }

    //Create menu options
    m_Options.clear();

    auto createOption = [this](const std::string& label, float yPos){
        SDL_Color normalColor = m_NormalColor;
        SDL_Color selectedColor = m_SelectedColor;

        auto normalTexture = CreateTextTexture(label, normalColor);
        auto selectedTexture = CreateTextTexture(label, selectedColor);
        if(normalTexture && selectedTexture){
            MenuOption option;
            option.Label = label;
            option.NormalTexture = normalTexture;
            option.SelectedTexture = selectedTexture;
            option.Position = Vector3D(0.0f, yPos, 0.0f);
            option.Width = static_cast<float>(normalTexture->GetWidth());
            option.Height = static_cast<float>(normalTexture->GetHeight());
            m_Options.push_back(option);
        }
    };

    //Position options vertically (top to bottom)
    float startY = 2.0f;
    float spacing = 2.5f;
    createOption("Play", startY + spacing * 2);
    createOption("Options", startY + spacing);
    createOption("Exit", startY);

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
 * @param text The string to render.
 * @param color The color of the text.
 * @return Shared pointer to the Texture2D, or nullptr on failure.
 */
std::shared_ptr<Texture2D> MenuScene::CreateTextTexture(const std::string& text, const SDL_Color& color){
    if(!m_Font || !m_Font->IsLoaded()){
        Log::Warning("Cannot render text: no font loaded.");
        return nullptr;
    }

    SDL_Surface* surface = m_Font->RenderText(text, color);
    if(!surface){
        Log::Error("RenderText returned null surface for: " + text);
        return nullptr;
    }

    auto texture = std::make_shared<Texture2D>();
    if(!texture->LoadFromSurface(surface)){
        Log::Error("LoadFromSurface failed for: " + text);
        SDL_DestroySurface(surface);
        return nullptr;
    }

    //Use nearest neighbor for sharp text rendering
    glBindTexture(GL_TEXTURE_2D, texture->GetRendererID());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    SDL_DestroySurface(surface);
    Log::Info("Text texture created: " + text);
    return texture;
}

/**
 * @brief Updates the menu: navigation and selection.
 * @param deltaTime Time elapsed since the last frame (unused).
 */
void MenuScene::Update(float deltaTime){
    //Keyboard navigation - Down
    if(Input::IsKeyPressed(SDL_SCANCODE_DOWN) || Input::IsKeyPressed(SDL_SCANCODE_S) ||
        GamepadManager::GetInstance().IsButtonPressed(0, SDL_GAMEPAD_BUTTON_DPAD_DOWN)){
        m_SelectedOption = (m_SelectedOption + 1) % static_cast<int>(m_Options.size());
    Log::Info("Selected: " + m_Options[m_SelectedOption].Label);
    }

    //Keyboard navigation - Up
    if(Input::IsKeyPressed(SDL_SCANCODE_UP) || Input::IsKeyPressed(SDL_SCANCODE_W) ||
        GamepadManager::GetInstance().IsButtonPressed(0, SDL_GAMEPAD_BUTTON_DPAD_UP)){
        m_SelectedOption = (m_SelectedOption - 1 + static_cast<int>(m_Options.size())) % static_cast<int>(m_Options.size());
    Log::Info("Selected: " + m_Options[m_SelectedOption].Label);
    }

    //Confirm selection with keyboard
    if(Input::IsKeyPressed(SDL_SCANCODE_RETURN) || Input::IsKeyPressed(SDL_SCANCODE_SPACE) ||
        GamepadManager::GetInstance().IsButtonPressed(0, SDL_GAMEPAD_BUTTON_EAST)){
        HandleSelection();
    }

    //Mouse click detection
    if(Input::IsMouseButtonPressed(1)){
        Vector2D mousePos = Input::GetMousePosition();
        int winWidth = Input::GetWindowWidth();
        int winHeight = Input::GetWindowHeight();

        //Convert to world coordinates (projection: -10 to 10)
        float worldX = (mousePos.x / winWidth) * 20.0f - 10.0f;
        float worldY = -(mousePos.y / winHeight) * 20.0f + 10.0f;

        for(size_t i = 0; i < m_Options.size(); ++i){
            const auto& opt = m_Options[i];
            float halfWidth = (opt.Width / opt.Height) * 1.5f * 0.5f;
            float halfHeight = 0.75f;

            float left = opt.Position.x - halfWidth;
            float right = opt.Position.x + halfWidth;
            float bottom = opt.Position.y - halfHeight;
            float top = opt.Position.y + halfHeight;

            if(worldX >= left && worldX <= right && worldY >= bottom && worldY <= top){
                m_SelectedOption = static_cast<int>(i);
                HandleSelection();
                break;
            }
        }
    }
}

/**
 * @brief Renders the menu.
 */
void MenuScene::Render(){
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for(size_t i = 0; i < m_Options.size(); ++i){
        bool isSelected = (static_cast<int>(i) == m_SelectedOption);
        RenderOption(m_Options[i], isSelected);
    }
}

/**
 * @brief Checks if the mouse is over a menu option.
 * @param option The menu option to check.
 * @return true if the mouse is over the option, false otherwise.
 */
bool MenuScene::IsMouseOverOption(const MenuOption& option) const{
    Vector2D mousePos = Input::GetMousePosition();
    int winWidth = Input::GetWindowWidth();
    int winHeight = Input::GetWindowHeight();

    float worldX = (mousePos.x / winWidth) * 20.0f - 10.0f;
    float worldY = -(mousePos.y / winHeight) * 20.0f + 10.0f;

    float halfWidth = (option.Width / option.Height) * 1.5f * 0.5f;
    float halfHeight = 0.75f;

    float left = option.Position.x - halfWidth;
    float right = option.Position.x + halfWidth;
    float bottom = option.Position.y - halfHeight;
    float top = option.Position.y + halfHeight;

    return (worldX >= left && worldX <= right && worldY >= bottom && worldY <= top);
}

/**
 * @brief Renders a single menu option.
 * @param option The menu option to render.
 * @param isSelected Whether this option is currently selected.
 */
void MenuScene::RenderOption(const MenuOption& option, bool isSelected){
    bool isHovered = IsMouseOverOption(option);
    std::shared_ptr<Texture2D> texture = (isSelected || isHovered)
    ? option.SelectedTexture
    : option.NormalTexture;

    if(!texture){
        Log::Warning("RenderOption: texture is null for " + option.Label);
        return;
    }

    //Build model matrix
    Matrix4 model = Matrix4::Translation(option.Position);
    float aspect = static_cast<float>(texture->GetWidth()) / static_cast<float>(texture->GetHeight());
    float height = 1.5f;
    float width = height * aspect;
    model = model * Matrix4::Scale(Vector3D(width, height, 1.0f));

    //View and projection (fixed orthographic)
    Matrix4 view = Matrix4::Identity();
    Matrix4 projection = Matrix4::Orthographic(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);

    Renderer::DrawTexturedQuad(m_Shader, m_VertexArray, m_IndexBuffer,
                               *texture, model, view, projection, 1.0f, 1.0f);
}

/**
 * @brief Handles the selection of the current menu option.
 */
void MenuScene::HandleSelection(){
    if(m_Options.empty()) return;

    const std::string& selected = m_Options[m_SelectedOption].Label;

    if(selected == "Play"){
        Log::Info("Starting Demo 2D...");
        auto selector = std::make_unique<LevelSelectorScene>(
            m_SceneManager, m_Shader, m_VertexArray,
            m_IndexBuffer, m_Camera, m_LineShader);
        m_SceneManager.SetScene(std::move(selector));
    } else if(selected == "Options"){
        Log::Info("Options menu to be implemented later.");
    } else if(selected == "Exit"){
        Log::Info("Exiting game...");
        SDL_Event quitEvent{ .type = SDL_EVENT_QUIT };
        SDL_PushEvent(&quitEvent);
    }
}
