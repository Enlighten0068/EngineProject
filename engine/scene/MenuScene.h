#pragma once

#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include <vector>
#include <string>

class MenuScene : public Scene{
public:
    MenuScene(SceneManager& sceneManager);
    void OnEnter() override;
    void OnExit() override;
    void Update(float deltaTime) override;
    void Render() override;
    std::string GetName() const override { return "Menu"; }

private:
    SceneManager& m_SceneManager;
    int m_SelectedOption = 0;
    std::vector<std::string> m_Options = {"Demo 2D", "Exit"};
    void HandleSelection();
};
