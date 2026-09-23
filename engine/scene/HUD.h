#pragma once

#include "graphics/Font.h"
#include "graphics/Texture2D.h"
#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Simple heads-up display overlay for rendering text on top of scenes.
 *
 * The HUD renders a list of text lines at the top-right corner of the screen
 * using screen-space pixel coordinates. Text textures are cached by content.
 *
 * @note Rendered by Application after the scene, so it always appears on top.
 * @see Scene::GetHUDLines
 */
class HUD{
public:
    HUD();
    ~HUD();

    /**
     * @brief Initializes the HUD with a font.
     * @param fontPath Path to the TTF font file.
     * @param fontSize Font size in points.
     * @return true if initialized successfully.
     */
    bool Initialize(const std::string& fontPath, int fontSize);

    /**
     * @brief Releases all HUD resources.
     */
    void Shutdown();

    /**
     * @brief Renders text lines at the top-right corner of the screen.
     * @param lines Text lines (top to bottom).
     * @param shader Shader used to render text.
     * @param va Vertex array.
     * @param ib Index buffer.
     * @param windowWidth Current window width in pixels.
     * @param windowHeight Current window height in pixels.
     */
    void Render(const std::vector<std::string>& lines,
                Shader& shader, VertexArray& va, IndexBuffer& ib,
                int windowWidth, int windowHeight);

private:
    std::unique_ptr<Font> m_Font;
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_TextureCache;

    std::shared_ptr<Texture2D> GetOrCreateTexture(const std::string& text,
                                                  const SDL_Color& color);
};
