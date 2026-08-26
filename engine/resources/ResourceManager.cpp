#include "resources/ResourceManager.h"
#include "diagnostics/Log.h"
#include <format>

ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& filepath) {
    auto it = m_Textures.find(filepath);
    if (it != m_Textures.end()) {
        Log::Info(std::format("Texture already loaded: {}", filepath));
        return it->second;
    }

    auto texture = std::make_shared<Texture2D>();
    if (!texture->Load(filepath)) {
        Log::Error(std::format("Failed to load texture: {}", filepath));
        return nullptr;
    }

    m_Textures[filepath] = texture;
    Log::Info(std::format("Texture loaded and cached: {}", filepath));
    return texture;
}

std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string& name,
                                                    const std::string& vertexSource,const std::string& fragmentSource) {
    auto it = m_Shaders.find(name);
    if (it != m_Shaders.end()) {
        Log::Info(std::format("Shader already loaded: {}", name));
        return it->second;
    }

    // Compilar novo shader
    auto shader = std::make_shared<Shader>();
    if (!shader->Compile(vertexSource, fragmentSource)) {
        Log::Error(std::format("Failed to compile shader: {}", name));
        return nullptr;
    }

    m_Shaders[name] = shader;
    Log::Info(std::format("Shader loaded and cached: {}", name));
    return shader;
}

void ResourceManager::Clear() {
    m_Textures.clear();
    m_Shaders.clear();
    Log::Info("ResourceManager cleared.");

}
