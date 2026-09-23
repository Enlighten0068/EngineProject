#pragma once

#include "graphics/Font.h"
#include "graphics/Texture2D.h"
#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include "scene/Camera2D.h"
#include "math/Vector3D.h"
#include <memory>
#include <vector>
#include <string>

/**
 * @brief Scene that lists all registered levels and lets the player choose one.
 *
 * The list is populated automatically from the LevelRegistry, so adding a new
 * level only requires creating a new .cpp with REGISTER_LEVEL and recompiling.
 *
 * Navigation: Up/Down (or W/S) to select, Enter/Space to confirm,
 * ESC to go back to the main menu. Mouse hover highlights the option under
 * the cursor, and clicking loads it.
 *
 * @see LevelRegistry, MenuScene, BaseLevelScene
 */
class LevelSelectorScene : public Scene{
public:
    LevelSelectorScene(SceneManager& sceneManager, Shader& shader, VertexArray& va,
                       IndexBuffer& ib, Camera2D& camera, Shader& lineShader);
    ~LevelSelectorScene();

    void OnEnter() override;
    void OnExit() override;
    void Update(float deltaTime) override;
    void Render() override;
    std::string GetName() const override{ return "LevelSelector"; }

    /**
     * @brief Returns the HUD lines to show in this scene.
     */
    std::vector<std::string> GetHUDLines() const override{
        return {
            "F11: Toggle Fullscreen",
            "ESC: Back to Menu"
        };
    }

private:
    /**
     * @brief A single menu option (text label with two textures).
     */
    struct Option{
        std::string Label; //Display text
        std::shared_ptr<Texture2D> NormalTexture; //Normal state texture
        std::shared_ptr<Texture2D> SelectedTexture; //Selected/hover state texture
        Vector3D Position; //World position
        float Width = 0.0f; //Texture width
        float Height = 0.0f; //Texture height
    };

    SceneManager& m_SceneManager;
    Shader& m_Shader;
    VertexArray& m_VertexArray;
    IndexBuffer& m_IndexBuffer;
    Camera2D& m_Camera;
    Shader& m_LineShader;

    std::unique_ptr<Font> m_Font; //Font for rendering text
    std::vector<Option> m_Options; //Registered levels as options
    int m_SelectedOption = 0; //Currently selected index

    SDL_Color m_NormalColor = {255, 255, 255, 255}; //Normal text color
    SDL_Color m_SelectedColor = {255, 255, 0, 255}; //Selected text color

    std::shared_ptr<Texture2D> CreateTextTexture(const std::string& text, const SDL_Color& color);
    void RenderOption(const Option& option, bool isSelected);
    bool IsMouseOverOption(const Option& option) const;
    void HandleSelection();
};
