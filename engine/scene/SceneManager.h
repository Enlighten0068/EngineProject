#pragma once

#include "scene/Scene.h"
#include <memory>
#include <stack>

/**
 * @brief Manages a stack of scenes.
 *
 * The SceneManager handles scene transitions by maintaining a stack of scenes.
 * Only the top scene is active (receives Update and Render calls).
 *
 * @note Supports pushing, popping, and setting scenes.
 * @see Scene
 */
class SceneManager{
public:
    /**
     * @brief Replaces the current scene with a new one.
     *
     * Exits the current scene (if any) and enters the new scene.
     *
     * @param scene The new scene to set.
     */
    void SetScene(std::unique_ptr<Scene> scene);

    /**
     * @brief Pushes a new scene onto the stack.
     *
     * The current scene is paused (not exited) and the new scene becomes active.
     *
     * @param scene The scene to push.
     */
    void PushScene(std::unique_ptr<Scene> scene);

    /**
     * @brief Pops the current scene from the stack.
     *
     * Exits the current scene and returns to the previous scene.
     * Does nothing if only one scene remains.
     */
    void PopScene();

    /**
     * @brief Updates the active scene.
     * @param deltaTime Time elapsed since the last frame.
     */
    void Update(float deltaTime);

    /**
     * @brief Renders the active scene.
     */
    void Render();

    /**
     * @brief Gets the current active scene.
     * @return Pointer to the current scene, or nullptr if no scene is active.
     */
    Scene* GetCurrentScene() const{
        return m_SceneStack.empty() ? nullptr : m_SceneStack.top().get();
    }

private:
    std::stack<std::unique_ptr<Scene>> m_SceneStack; //Stack of scenes
};
