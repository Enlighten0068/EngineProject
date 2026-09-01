#include "diagnostics/Log.h"
#include "scene/SceneManager.h"

void SceneManager::SetScene(std::unique_ptr<Scene> scene){
    if (!m_SceneStack.empty()){
        m_SceneStack.top()->OnExit();
        m_SceneStack.pop();
    }

    if (scene){
        scene->OnEnter();
        m_SceneStack.push(std::move(scene));
        Log::Info("Scene set: " + m_SceneStack.top()->GetName());
    }
}

void SceneManager::PushScene(std::unique_ptr<Scene> scene){
    if (scene) {
        scene->OnEnter();
        m_SceneStack.push(std::move(scene));
        Log::Info("Scene pushed: " + m_SceneStack.top()->GetName());
    }
}

void SceneManager::PopScene(){
    if (m_SceneStack.size() > 1) {
        m_SceneStack.top()->OnExit();
        m_SceneStack.pop();
        Log::Info("Scene popped. Current: " + m_SceneStack.top()->GetName());
    } else Log::Warning("Cannot pop the last scene.");

}

void SceneManager::Update(float deltaTime){
    if (!m_SceneStack.empty()) m_SceneStack.top()->Update(deltaTime);
}

void SceneManager::Render(){
    if (!m_SceneStack.empty()) m_SceneStack.top()->Render();

}
