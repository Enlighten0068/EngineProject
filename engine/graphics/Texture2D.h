#pragma once

#include <cstdint>
#include <string>

/**
 * @brief 2D texture wrapper for OpenGL textures.
 *
 * Loads images from files (using stb_image) and manages the OpenGL texture object.
 * Supports wrapping modes (GL_REPEAT) and mipmap generation.
 *
 * @note Textures are loaded with vertical flip enabled (stbi_set_flip_vertically_on_load).
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
     * @brief Binds the texture to a texture unit.
     * @param slot Texture unit index.
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

private:
    uint32_t m_RendererID;

    //Height and width in pixels
    int m_Width;
    int m_Height;

    int m_Channels;
};
