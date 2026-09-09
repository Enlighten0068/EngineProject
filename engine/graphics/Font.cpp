#include "graphics/Font.h"
#include "diagnostics/Log.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <format>

Font::Font() = default;

Font::~Font(){
    if(m_Font){
        TTF_CloseFont(m_Font);
        m_Font = nullptr;
    }
}

/**
 * @brief Loads a TrueType font from a file.
 * @param filepath Path to the TTF font file.
 * @param pointSize Font size in points.
 * @return true if the font was loaded successfully, false otherwise.
 */
bool Font::Load(const std::string& filepath, int pointSize){
    m_Font = TTF_OpenFont(filepath.c_str(), pointSize);
    if(!m_Font){
        Log::Error(std::format("Failed to load font '{}': {}", filepath, SDL_GetError()));
        return false;
    }

    Log::Info(std::format("Font loaded: {} ({}pt)", filepath, pointSize));
    return true;
}

/**
 * @brief Renders a string to an SDL_Surface.
 *
 * Uses SDL_ttf to render the text with the specified color.
 * The surface is in SDL_PIXELFORMAT_ARGB8888 format, which can be
 * converted to an OpenGL texture.
 *
 * @param text The string to render.
 * @param color The color of the text.
 * @return SDL_Surface* containing the rendered text, or nullptr on failure.
 */
SDL_Surface* Font::RenderText(const std::string& text, const SDL_Color& color) const{
    if(!m_Font){
        Log::Warning("Attempted to render text with no font loaded.");
        return nullptr;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(m_Font, text.c_str(), text.length(), color);
    if(!surface){
        Log::Error(std::format("Failed to render text '{}': {}", text, SDL_GetError()));
        return nullptr;
    }

    return surface;
}
