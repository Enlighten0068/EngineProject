#pragma once

#include "graphics/Texture2D.h"
#include "graphics/Shader.h"
#include <memory>
#include <string>
#include <unordered_map>

class ResourceManager {
public:
    static ResourceManager& GetInstance();

    std::shared_ptr<Texture2D> LoadTexture(const std::string& filepath);

    std::shared_ptr<Shader> LoadShader(const std::string& name,
                                       const std::string& vertexSource, const std::string& fragmentSource);

    void Clear();

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_Textures;
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
};
