#include "core/Input.h"
#include "demo/Demo2DScene.h"
#include "diagnostics/Log.h"
#include "graphics/GraphicsContext.h"
#include "scene/MenuScene.h"
#include <SDL3/SDL.h>

MenuScene::MenuScene(SceneManager& sceneManager) : m_SceneManager(sceneManager){}

void MenuScene::OnEnter() { Log::Info("Entering MenuScene"); }
void MenuScene::OnExit() { Log::Info("Exiting MenuScene"); }

void MenuScene::Update(float deltaTime) {
    if (Input::IsKeyPressed(SDL_SCANCODE_DOWN) || Input::IsKeyPressed(SDL_SCANCODE_S)){
        m_SelectedOption = (m_SelectedOption + 1) % m_Options.size();
        Log::Info("Selected: " + m_Options[m_SelectedOption]);
    }
    if (Input::IsKeyPressed(SDL_SCANCODE_UP) || Input::IsKeyPressed(SDL_SCANCODE_W)){
        m_SelectedOption = (m_SelectedOption - 1 + m_Options.size()) % m_Options.size();
        Log::Info("Selected: " + m_Options[m_SelectedOption]);
    }
    if (Input::IsKeyPressed(SDL_SCANCODE_RETURN) || Input::IsKeyPressed(SDL_SCANCODE_SPACE)){
        HandleSelection();
    }
}

void MenuScene::Render(){
    Log::InfoThrottled("Rendering menu (use arrows, Enter to select)", "menu_render", 2.0f);
}

void MenuScene::HandleSelection(){
    switch (m_SelectedOption) {
        case 0:{
            Log::Info("Starting Demo 2D... (not implemented)");
            break;
        }
        case 1:{
            Log::Info("Exiting game...");
            SDL_Event quitEvent{ .type = SDL_EVENT_QUIT };
            SDL_PushEvent(&quitEvent);
            break;
        }
    }
}
