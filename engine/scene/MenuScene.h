#pragma once

#include "graphics/Font.h"
#include "graphics/Texture2D.h"
#include "graphics/Shader.h"
#include "math/Vector3D.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include <vector>
#include <string>

/**
 * @brief Main menu scene with visual text rendering.
 *
 * The MenuScene provides a graphical menu with options rendered
 * using SDL_ttf. Navigation is done with arrow keys / D-pad,
 * and selection with Enter / Space / A button.
 *
 * @note This scene requires a font file to be present in assets.
 * @see Font, SceneManager
 */
class MenuScene : public Scene{
public:
    MenuScene(SceneManager& sceneManager, Shader& shader, VertexArray& va, IndexBuffer& ib,
              Camera2D& camera, Shader& lineShader);
    ~MenuScene();

    void OnEnter() override;
    void OnExit() override;
    void Update(float deltaTime) override;
    void Render() override;
    std::string GetName() const override{ return "Menu"; }

private:

    /**
     * @brief Menu option data.
     */
    struct MenuOption{
        std::string Label; //Display text for the option
        std::shared_ptr<Texture2D> Texture; //Rendered texture of the text
        Vector3D Position; //Position in world coordinates
        float Width; //Texture width.
        float Height; //Texture height.
    };

    Camera2D& m_Camera;
    Shader& m_LineShader;

    SceneManager& m_SceneManager;
    Shader& m_Shader;
    VertexArray& m_VertexArray;
    IndexBuffer& m_IndexBuffer;

    std::unique_ptr<Font> m_Font;
    std::vector<MenuOption> m_Options;
    int m_SelectedOption = 0;

    std::shared_ptr<Texture2D> m_BackgroundTexture; //Background texture
    SDL_Color m_NormalColor = {255, 255, 255, 255}; //Color for text
    SDL_Color m_SelectedColor = {255, 255, 0, 255}; //Color for selected text

    /**
     * @brief Creates a texture from rendered text.
     *
     * Renders the text using the font and converts the surface to an OpenGL texture.
     *
     * @param text The string to render.
     * @param color The color of the text.
     * @return Shared pointer to the created Texture2D, or nullptr on failure.
     */
    std::shared_ptr<Texture2D> CreateTextTexture(const std::string& text, const SDL_Color& color);

    /**
     * @brief Handles the selection of the current menu option.
     */
    void HandleSelection();

    /**
     * @brief Renders a single menu option.
     * @param option The menu option to render.
     * @param isSelected Whether this option is currently selected.
     */
    void RenderOption(const MenuOption& option, bool isSelected);
};
