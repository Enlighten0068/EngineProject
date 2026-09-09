#include "diagnostics/Log.h"
#include "scene/SceneManager.h"

/**
 * @brief Replaces the current scene with a new one.
 * @param scene The new scene to set.
 */
void SceneManager::SetScene(std::unique_ptr<Scene> scene){
    //Exit and remove current scene
    if(!m_SceneStack.empty()){
        m_SceneStack.top()->OnExit();
        m_SceneStack.pop();
    }

    //Enter and push new scene
    if(scene){
        scene->OnEnter();
        m_SceneStack.push(std::move(scene));
        Log::Info("Scene set: " + m_SceneStack.top()->GetName());
    }
}

/**
 * @brief Pushes a new scene onto the stack.
 * @param scene The scene to push.
 */
void SceneManager::PushScene(std::unique_ptr<Scene> scene){
    if(scene){
        scene->OnEnter();
        m_SceneStack.push(std::move(scene));
        Log::Info("Scene pushed: " + m_SceneStack.top()->GetName());
    }
}

/**
 * @brief Pops the current scene from the stack.
 */
void SceneManager::PopScene(){
    if(m_SceneStack.size() > 1){
        m_SceneStack.top()->OnExit();
        m_SceneStack.pop();
        Log::Info("Scene popped. Current: " + m_SceneStack.top()->GetName());
    } else{
        Log::Warning("Cannot pop the last scene.");
    }
}

/**
 * @brief Updates the active scene.
 * @param deltaTime Time elapsed since the last frame.
 */
void SceneManager::Update(float deltaTime){
    if(!m_SceneStack.empty()) m_SceneStack.top()->Update(deltaTime);
}

/**
 * @brief Renders the active scene.
 */
void SceneManager::Render(){
    if(!m_SceneStack.empty()) m_SceneStack.top()->Render();
}
