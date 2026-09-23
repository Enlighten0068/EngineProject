#include "scene/HUD.h"
#include "diagnostics/Log.h"
#include "renderer/Renderer.h"
#include "math/Matrix4.h"
#include <glad/glad.h>
#include <format>

namespace{
    constexpr float MARGIN  = 12.0f;    //pixels from screen edges
    constexpr float PADDING = 4.0f;     //pixels between lines
    constexpr float SHADOW_OFFSET = 2.0f;
    const SDL_Color TEXT_COLOR   = {255, 255, 255, 255};
    const SDL_Color SHADOW_COLOR = {0, 0, 0, 220};
}

HUD::HUD() = default;

HUD::~HUD(){ Shutdown(); }

bool HUD::Initialize(const std::string& fontPath, int fontSize){
    m_Font = std::make_unique<Font>();
    if(!m_Font->Load(fontPath, fontSize)){
        Log::Warning("HUD: failed to load font: " + fontPath);
        m_Font.reset();
        return false;
    }
    Log::Info("HUD initialized.");
    return true;
}

void HUD::Shutdown(){
    m_TextureCache.clear();
    m_Font.reset();
}

std::shared_ptr<Texture2D> HUD::GetOrCreateTexture(const std::string& text,
                                                   const SDL_Color& color){
    std::string key = text + "|" +
    std::to_string(color.r) + "," +
    std::to_string(color.g) + "," +
    std::to_string(color.b) + "," +
    std::to_string(color.a);

    auto it = m_TextureCache.find(key);
    if(it != m_TextureCache.end()) return it->second;

    if(!m_Font || !m_Font->IsLoaded()) return nullptr;

    SDL_Surface* surface = m_Font->RenderText(text, color);
    if(!surface) return nullptr;

    auto texture = std::make_shared<Texture2D>();
    if(!texture->LoadFromSurface(surface)){
        SDL_DestroySurface(surface);
        return nullptr;
    }

    glBindTexture(GL_TEXTURE_2D, texture->GetRendererID());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    SDL_DestroySurface(surface);
    m_TextureCache[key] = texture;
    return texture;
}

void HUD::Render(const std::vector<std::string>& lines,
                Shader& shader, VertexArray& va, IndexBuffer& ib,
                int windowWidth, int windowHeight){
    if(!m_Font || !m_Font->IsLoaded() || lines.empty()) return;

    //Screen-space orthographic projection:
    //(0, 0) = bottom-left, (W, H) = top-right
    Matrix4 view = Matrix4::Identity();
    Matrix4 projection = Matrix4::Orthographic(
        0.0f, static_cast<float>(windowWidth),
                                                0.0f, static_cast<float>(windowHeight),
                                                -1.0f, 1.0f);

    float topY = static_cast<float>(windowHeight) - MARGIN;

    for(const auto& line : lines){
        auto texture = GetOrCreateTexture(line, TEXT_COLOR);
        if(!texture) continue;

        float w = static_cast<float>(texture->GetWidth());
        float h = static_cast<float>(texture->GetHeight());

        //Right-aligned: right edge at (W - MARGIN), top at topY
        float x = static_cast<float>(windowWidth) - MARGIN - w;
        float y = topY - h;  //bottom-left of the text quad

        //Shadow (offset)
        auto shadow = GetOrCreateTexture(line, SHADOW_COLOR);
        if(shadow){
            Matrix4 shadowModel = Matrix4::Translation(
                Vector3D(x + w * 0.5f + SHADOW_OFFSET, y + h * 0.5f - SHADOW_OFFSET, 0.0f));
            shadowModel = shadowModel * Matrix4::Scale(Vector3D(w, h, 1.0f));
            Renderer::DrawTexturedQuad(shader, va, ib, *shadow,
                                        shadowModel, view, projection, 1.0f, 1.0f);
        }

        //Main text
        Matrix4 model = Matrix4::Translation(Vector3D(x + w * 0.5f, y + h * 0.5f, 0.0f));
        model = model * Matrix4::Scale(Vector3D(w, h, 1.0f));
        Renderer::DrawTexturedQuad(shader, va, ib, *texture,
                                    model, view, projection, 1.0f, 1.0f);

        topY -= h + PADDING;
    }
}
