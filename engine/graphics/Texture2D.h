#pragma once

#include <SDL3/SDL.h>
#include <cstdint>
#include <string>

/**
 * @brief 2D texture wrapper for OpenGL textures.
 *
 * Loads images from files (using stb_image) and manages the OpenGL texture object.
 * Supports wrapping modes (GL_REPEAT) and mipmap generation.
 *
 * @note Textures are loaded with vertical flip enabled (stbi_set_flip_vertically_on_load).
 * @see ResourceManager
 */
class Texture2D{
public:
    Texture2D();
    ~Texture2D();

    /**
     * @brief Loads a texture from a file.
     *
     * Supports common formats (.png, .jpg, .bmp, etc.) via stb_image.
     *
     * @param filepath Path to the image file.
     * @return true if the texture was loaded successfully, false otherwise.
     */
    bool Load(const std::string& filepath);

    /**
     * @brief Loads a texture from an SDL_Surface.
     *
     * This method is useful for rendering text from SDL_ttf.
     *
     * @param surface The SDL_Surface containing the image data.
     * @return true if the texture was loaded successfully, false otherwise.
     * @note The surface is NOT freed by this method; the caller must free it.
     */
    bool LoadFromSurface(SDL_Surface* surface);

    /**
     * @brief Binds the texture to a texture unit.
     * @param slot Texture unit index (default: 0).
     */
    void Bind(uint32_t slot = 0) const;

    /**
     * @brief Unbinds the texture.
     */
    void Unbind() const;

    //Getters
    int GetWidth() const;
    int GetHeight() const;
    int GetChannels() const;
    uint32_t GetRendererID() const{ return m_RendererID; }

private:
    uint32_t m_RendererID; //OpenGL texture ID
    int m_Width; //Texture width in pixels
    int m_Height; //Texture height in pixels
    int m_Channels; //Number of color channels (3 = RGB, 4 = RGBA)
};
