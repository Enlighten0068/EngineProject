#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <memory>

/**
 * @brief Wrapper for a TrueType font using SDL_ttf.
 *
 * This class manages loading a font from a file and rendering text
 * to OpenGL textures. It supports setting font size and style.
 *
 * @note The font is loaded from a TTF file and rendered to SDL_Surface,
 *       which can then be converted to an OpenGL texture.
 * @see Texture2D
 */
class Font{
public:
    Font();
    ~Font();

    /**
     * @brief Loads a TrueType font from a file.
     * @param filepath Path to the TTF font file.
     * @param pointSize Font size in points.
     * @return true if the font was loaded successfully, false otherwise.
     */
    bool Load(const std::string& filepath, int pointSize);

    /**
     * @brief Renders a string to an SDL_Surface.
     *
     * The surface can be converted to an OpenGL texture for rendering.
     *
     * @param text The string to render.
     * @param color The color of the text.
     * @return SDL_Surface* containing the rendered text, or nullptr on failure.
     *         The caller is responsible for freeing the surface.
     */
    SDL_Surface* RenderText(const std::string& text, const SDL_Color& color) const;

    /**
     * @brief Checks if a font is loaded.
     * @return true if a font is loaded, false otherwise.
     */
    bool IsLoaded() const{ return m_Font != nullptr; }

private:
    TTF_Font* m_Font = nullptr; //TTF font handle
};
