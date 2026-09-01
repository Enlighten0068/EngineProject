#pragma once

#include "scene/Scene.h"
#include <memory>
#include <stack>

class SceneManager {
public:
    void SetScene(std::unique_ptr<Scene> scene);
    void PushScene(std::unique_ptr<Scene> scene);
    void PopScene();
    void Update(float deltaTime);
    void Render();
    Scene* GetCurrentScene() const { return m_SceneStack.empty() ? nullptr : m_SceneStack.top().get(); }

private:
    std::stack<std::unique_ptr<Scene>> m_SceneStack;
};
