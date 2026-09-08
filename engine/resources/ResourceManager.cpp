#include "resources/ResourceManager.h"
#include "diagnostics/Log.h"
#include <format>

/**
 * @brief Returns the singleton instance of ResourceManager.
 *
 * The instance is created on first call and destroyed automatically
 * when the program exits.
 *
 * @return Reference to the ResourceManager instance.
 */
ResourceManager& ResourceManager::GetInstance(){
    static ResourceManager instance;
    return instance;
}

/**
 * @brief Loads a texture from a file path, with caching.
 *
 * This method checks the cache first to avoid loading the same texture twice.
 * If the texture is not already loaded, it creates a new Texture2D instance
 * and attempts to load the file. On success, the texture is added to the cache.
 *
 * @param filepath Path to the texture file.
 * @return Shared pointer to the loaded Texture2D, or nullptr on failure.
 */
std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& filepath){
    //Check if texture is cached
    auto it = m_Textures.find(filepath);
    if (it != m_Textures.end()){
        Log::Info(std::format("Texture already loaded: {}", filepath));
        return it->second;
    }

    //Load texture from disk
    auto texture = std::make_shared<Texture2D>();
    if (!texture->Load(filepath)){
        Log::Error(std::format("Failed to load texture: {}", filepath));
        return nullptr;
    }

    //Caches texture
    m_Textures[filepath] = texture;
    Log::Info(std::format("Texture loaded and cached: {}", filepath));
    return texture;
}

/**
 * @brief Loads or retrieves a shader program by name.
 *
 * This method caches shaders by name, allowing the same shader to be used
 * by multiple entities without recompiling. This is particularly useful for
 * shaders that are used frequently (e.g., the standard sprite shader).
 *
 * @param name Unique identifier for this shader.
 * @param vertexSource GLSL source code for the vertex shader.
 * @param fragmentSource GLSL source code for the fragment shader.
 * @return Shared pointer to the loaded Shader, or nullptr on compilation failure.
 */
std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string& name,
                                                    const std::string& vertexSource,const std::string& fragmentSource){
    auto it = m_Shaders.find(name);
    if (it != m_Shaders.end()){
        Log::Info(std::format("Shader already loaded: {}", name));
        return it->second;
    }

    auto shader = std::make_shared<Shader>();
    if (!shader->Compile(vertexSource, fragmentSource)){
        Log::Error(std::format("Failed to compile shader: {}", name));
        return nullptr;
    }

    m_Shaders[name] = shader;
    Log::Info(std::format("Shader loaded and cached: {}", name));
    return shader;
}

/**
 * @brief Clears all cached resources.
 *
 * Releases all shared pointers to textures and shaders, allowing their
 * memory to be freed if no other references exist. This is useful for
 * level transitions or application shutdown.
 */
void ResourceManager::Clear(){
    m_Textures.clear();
    m_Shaders.clear();
    Log::Info("ResourceManager cleared.");

}
