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
 * The list is populated automatically from the LevelRegistry.
 *
 * Navigation: Up/Down (or W/S) to select, Enter/Space to confirm, Esc to go back.
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

    std::vector<std::string> GetHUDLines() const override{
        return {
            "F11: Toggle Fullscreen",
            "ESC: Back to Menu"
        };
    }

private:
    struct Option{
        std::string Label;
        std::shared_ptr<Texture2D> NormalTexture;
        std::shared_ptr<Texture2D> SelectedTexture;
        Vector3D Position;
        float Width = 0.0f;
        float Height = 0.0f;
    };

    SceneManager& m_SceneManager;
    Shader& m_Shader;
    VertexArray& m_VertexArray;
    IndexBuffer& m_IndexBuffer;
    Camera2D& m_Camera;
    Shader& m_LineShader;

    std::unique_ptr<Font> m_Font;
    std::vector<Option> m_Options;
    int m_SelectedOption = 0;

    SDL_Color m_NormalColor = {255, 255, 255, 255};
    SDL_Color m_SelectedColor = {255, 255, 0, 255};

    std::shared_ptr<Texture2D> CreateTextTexture(const std::string& text, const SDL_Color& color);
    void RenderOption(const Option& option, bool isSelected);
    bool IsMouseOverOption(const Option& option) const;
    void HandleSelection();
};
