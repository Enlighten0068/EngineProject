#pragma once

#include <string>
#include <vector>

/**
 * @brief Abstract base class for all scenes in the engine.
 *
 * A scene represents a distinct game state or screen (e.g., menu, gameplay, pause).
 * Each scene must implement lifecycle methods: OnEnter, OnExit, Update, and Render.
 *
 * @note Scenes are managed by the SceneManager.
 * @see SceneManager
 */
class Scene{
public:
    virtual ~Scene() = default;

    /**
     * @brief Called when the scene becomes active.
     *
     * Use this method to initialize scene-specific resources.
     */
    virtual void OnEnter() = 0;

    /**
     * @brief Called when the scene is exited.
     *
     * Use this method to clean up scene-specific resources.
     */
    virtual void OnExit() = 0;

    /**
     * @brief Updates the scene logic.
     * @param deltaTime Time elapsed since the last frame.
     */
    virtual void Update(float deltaTime) = 0;

    /**
     * @brief Renders the scene.
     */
    virtual void Render() = 0;

    /**
     * @brief Gets the name of the scene.
     * @return String containing the scene name.
     */
    virtual std::string GetName() const = 0;

    /**
     * @brief Called when the window is resized.
     *
     * The scene decides how to react (e.g., recompute camera projection).
     *
     * @param width New window width in pixels.
     * @param height New window height in pixels.
     */
    virtual void OnResize(int width, int height){ (void)width; (void)height; }

    /**
     * @brief Returns the lines to be rendered by the HUD overlay.
     *
     * The default implementation returns only the F11 hint. Scenes that
     * have additional shortcuts (e.g., ESC to exit) should override this
     * method and append their own lines.
     *
     * @return Vector of strings to display in the HUD.
     * @see HUD, Application::Render
     */
    virtual std::vector<std::string> GetHUDLines() const{
        return { "F11: Toggle Fullscreen" };
    }
};
