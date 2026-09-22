#include "level/LevelSelectorScene.h"
#include "core/Input.h"
#include "core/GamepadManager.h"
#include "diagnostics/Log.h"
#include "level/LevelRegistry.h"
#include "renderer/Renderer.h"
#include "scene/BaseLevelScene.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <format>

LevelSelectorScene::LevelSelectorScene(SceneManager& sceneManager, Shader& shader,
                                       VertexArray& va, IndexBuffer& ib,
                                       Camera2D& camera, Shader& lineShader)
: m_SceneManager(sceneManager), m_Shader(shader), m_VertexArray(va),
m_IndexBuffer(ib), m_Camera(camera), m_LineShader(lineShader){}

LevelSelectorScene::~LevelSelectorScene(){ OnExit(); }

void LevelSelectorScene::OnEnter(){
    Log::Info("Entering LevelSelectorScene");

    m_Font = std::make_unique<Font>();
    if(!m_Font->Load("assets/fonts/arial.ttf", 48)){
        Log::Error("Failed to load font.");
    }

    m_Options.clear();
    const auto& levels = LevelRegistry::GetInstance().GetLevels();

    Log::Info(std::format("LevelSelector: {} levels registered", levels.size()));

    if(levels.empty()){
        Log::Warning("No levels registered!");
        return;
    }

    float startY = 3.0f;
    float spacing = 2.0f;

    for(size_t i = 0; i < levels.size(); ++i){
        const auto& lvl = levels[i];
        std::string label = lvl.Name;
        if(lvl.IsNew) label += " [NEW]";

        auto normalTex   = CreateTextTexture(label, m_NormalColor);
        auto selectedTex = CreateTextTexture(label, m_SelectedColor);
        if(!normalTex || !selectedTex) continue;

        Option opt;
        opt.Label = lvl.Name;
        opt.NormalTexture = normalTex;
        opt.SelectedTexture = selectedTex;
        opt.Position = Vector3D(0.0f, startY - i * spacing, 0.0f);
        opt.Width = static_cast<float>(normalTex->GetWidth());
        opt.Height = static_cast<float>(normalTex->GetHeight());
        m_Options.push_back(opt);
    }

    m_SelectedOption = 0;
}

void LevelSelectorScene::OnExit(){
    m_Options.clear();
    m_Font.reset();
}

std::shared_ptr<Texture2D> LevelSelectorScene::CreateTextTexture(const std::string& text,
                                                                 const SDL_Color& color){
    if(!m_Font || !m_Font->IsLoaded()) return nullptr;

    SDL_Surface* surface = m_Font->RenderText(text, color);
    if(!surface) return nullptr;

    auto texture = std::make_shared<Texture2D>();
    if(!texture->LoadFromSurface(surface)){
        SDL_DestroySurface(surface);
        return nullptr;
    }

    glBindTexture(GL_TEXTURE_2D, texture->GetRendererID());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    SDL_DestroySurface(surface);
    return texture;
}

void LevelSelectorScene::Update(float deltaTime){
    if(m_Options.empty()) return;

    int n = static_cast<int>(m_Options.size());

    //Navigation down
    if(Input::IsKeyPressed(SDL_SCANCODE_DOWN) || Input::IsKeyPressed(SDL_SCANCODE_S) ||
        GamepadManager::GetInstance().IsButtonPressed(0, SDL_GAMEPAD_BUTTON_DPAD_DOWN)){
        m_SelectedOption = (m_SelectedOption + 1) % n;
    }

    //Navigation up
    if(Input::IsKeyPressed(SDL_SCANCODE_UP) || Input::IsKeyPressed(SDL_SCANCODE_W) ||
        GamepadManager::GetInstance().IsButtonPressed(0, SDL_GAMEPAD_BUTTON_DPAD_UP)){
        m_SelectedOption = (m_SelectedOption - 1 + n) % n;
    }

    //Confirm
    if(Input::IsKeyPressed(SDL_SCANCODE_RETURN) || Input::IsKeyPressed(SDL_SCANCODE_SPACE) ||
        GamepadManager::GetInstance().IsButtonPressed(0, SDL_GAMEPAD_BUTTON_EAST)){
        HandleSelection();
    }
}

void LevelSelectorScene::Render(){
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for(size_t i = 0; i < m_Options.size(); ++i){
        bool isSelected = (static_cast<int>(i) == m_SelectedOption);
        RenderOption(m_Options[i], isSelected);
    }
}

bool LevelSelectorScene::IsMouseOverOption(const Option& option) const{
    Vector2D mousePos = Input::GetMousePosition();
    int winWidth = Input::GetWindowWidth();
    int winHeight = Input::GetWindowHeight();
    float worldX = (mousePos.x / winWidth) * 20.0f - 10.0f;
    float worldY = -(mousePos.y / winHeight) * 20.0f + 10.0f;

    float halfWidth = (option.Width / option.Height) * 1.2f * 0.5f;
    float halfHeight = 0.6f;

    float left   = option.Position.x - halfWidth;
    float right  = option.Position.x + halfWidth;
    float bottom = option.Position.y - halfHeight;
    float top    = option.Position.y + halfHeight;

    return (worldX >= left && worldX <= right && worldY >= bottom && worldY <= top);
}

void LevelSelectorScene::RenderOption(const Option& option, bool isSelected){
    bool isHovered = IsMouseOverOption(option);
    auto texture = (isSelected || isHovered) ? option.SelectedTexture : option.NormalTexture;
    if(!texture) return;

    Matrix4 model = Matrix4::Translation(option.Position);
    float aspect = static_cast<float>(texture->GetWidth()) / static_cast<float>(texture->GetHeight());
    float height = 1.2f;
    float width = height * aspect;
    model = model * Matrix4::Scale(Vector3D(width, height, 1.0f));

    Matrix4 view = Matrix4::Identity();
    Matrix4 projection = Matrix4::Orthographic(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);

    Renderer::DrawTexturedQuad(m_Shader, m_VertexArray, m_IndexBuffer,
                            *texture, model, view, projection, 1.0f, 1.0f);
}

void LevelSelectorScene::HandleSelection(){
    if(m_Options.empty()) return;

    const auto& levels = LevelRegistry::GetInstance().GetLevels();
    if(m_SelectedOption >= static_cast<int>(levels.size())) return;

    const auto& def = levels[m_SelectedOption];
    Log::Info("Loading level: " + def.Name);

    auto scene = std::make_unique<BaseLevelScene>(
        m_Shader, m_VertexArray, m_IndexBuffer,
        m_Camera, m_LineShader, m_SceneManager,
        def.Build, def.Name, def.Mode);
    m_SceneManager.SetScene(std::move(scene));
}
