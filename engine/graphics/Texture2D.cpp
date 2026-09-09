#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "graphics/Texture2D.h"
#include "diagnostics/Log.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <format>
#include <cstdint>

Texture2D::Texture2D() : m_RendererID(0),m_Width(0), m_Height(0), m_Channels(0){
}

Texture2D::~Texture2D(){
    if (m_RendererID) glDeleteTextures(1, &m_RendererID);

}

/**
 * @brief Loads a texture from a file using stb_image.
 *
 * The image is flipped vertically to match OpenGL's coordinate system
 * (origin at bottom-left). The texture is configured with:
 * GL_REPEAT for wrapping (S and T axes)
 * GL_LINEAR_MIPMAP_LINEAR for minification
 * GL_LINEAR for magnification
 *
 * @param filepath Path to the image file.
 * @return true if the texture was loaded successfully, false otherwise.
 */
bool Texture2D::Load(const std::string& filepath){
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_Channels, 0);

    if (!data){
        Log::Error(std::format("Failed loading texture: {}", filepath));
        return false;
    }

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    //Texture Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLenum format = (m_Channels == 4) ? GL_RGBA : GL_RGB;

    //Color system - RGBA for opacity/transparency
    if (m_Channels == 4) format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    Log::Info(std::format("Texture loaded: {} ({}x{})", filepath, m_Width, m_Height));

    return true;
}

/**
 * @brief Loads a texture from an SDL_Surface.
 */
bool Texture2D::LoadFromSurface(SDL_Surface* surface){
    if (!surface){
        Log::Error("Cannot load texture from null surface.");
        return false;
    }

    //Convert to RGBA format for OpenGL
    SDL_Surface* surfaceRGBA = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_ABGR8888);
    if (!surfaceRGBA){
        Log::Error(std::format("Failed to convert surface format: {}", SDL_GetError()));
        return false;
    }

    m_Width = surfaceRGBA->w;
    m_Height = surfaceRGBA->h;
    m_Channels = 4;

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int w = surfaceRGBA->w;
    int h = surfaceRGBA->h;
    int pitch = w * 4;
    uint8_t* pixels = static_cast<uint8_t*>(surfaceRGBA->pixels);
    std::vector<uint8_t> flippedData(pitch * h);
    for (int y = 0; y < h; ++y){
        memcpy(&flippedData[(h - 1 - y) * pitch], &pixels[y * pitch], pitch);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, flippedData.data());

    SDL_DestroySurface(surfaceRGBA);

    Log::Info(std::format("Texture loaded from surface ({}x{})", m_Width, m_Height));
    return true;
}

/**
 * @brief Binds the texture to a texture unit.
 * @param slot Texture unit index.
 */
void Texture2D::Bind(uint32_t slot) const{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

/**
 * @brief Unbinds the texture.
 */
void Texture2D::Unbind() const{
    glBindTexture(GL_TEXTURE_2D,0);
}

int Texture2D::GetWidth() const{
    return m_Width;
}

int Texture2D::GetHeight() const{
    return m_Height;
}

/**
 * @brief Returns the number of color channels.
 * @return 3 for RGB, 4 for RGBA.
 * @note This method is declared but not currently used.
 */
int Texture2D::GetChannels() const{
    return m_Channels;
}
