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
 * mouse clicks, and selection with Enter / Space / A button.
 *
 * @note This scene requires a font file to be present in assets.
 * @see Font, SceneManager
 */
class MenuScene : public Scene{
public:
    /**
     * @brief Constructs the MenuScene.
     * @param sceneManager Reference to the SceneManager for transitions.
     * @param shader Reference to the shader for rendering.
     * @param va Reference to the vertex array.
     * @param ib Reference to the index buffer.
     * @param camera Reference to the camera.
     * @param lineShader Reference to the line shader.
     */
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
        std::string Label; //Display text
        std::shared_ptr<Texture2D> NormalTexture; //Normal state texture
        std::shared_ptr<Texture2D> SelectedTexture; //Selected/hover state texture
        Vector3D Position; //World position
        float Width; //Texture width
        float Height; //Texture height
    };

    Camera2D& m_Camera; //Camera reference
    Shader& m_LineShader; //Line shader reference

    SceneManager& m_SceneManager; //Scene manager reference
    Shader& m_Shader; //Main shader
    VertexArray& m_VertexArray; //Vertex array
    IndexBuffer& m_IndexBuffer; //Index buffer

    std::unique_ptr<Font> m_Font; //Font for rendering text
    std::vector<MenuOption> m_Options; //Menu options
    int m_SelectedOption = 0; //Currently selected option index

    std::shared_ptr<Texture2D> m_BackgroundTexture; //Background texture
    SDL_Color m_NormalColor = {255, 255, 255, 255}; //Normal text color
    SDL_Color m_SelectedColor = {255, 255, 0, 255}; //Selected text color

    bool IsMouseOverOption(const MenuOption& option) const;

    std::shared_ptr<Texture2D> CreateTextTexture(const std::string& text, const SDL_Color& color);
    void HandleSelection();
    void RenderOption(const MenuOption& option, bool isSelected);
};
