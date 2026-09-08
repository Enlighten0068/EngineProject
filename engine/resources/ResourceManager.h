#pragma once

#include "graphics/Texture2D.h"
#include "graphics/Shader.h"
#include <memory>
#include <string>
#include <unordered_map>

/**
 * @brief Singleton resource manager for loading and caching assets.
 *
 * The ResourceManager provides a centralized system for loading and caching
 * assets such as textures and shaders. It ensures that each asset is loaded
 * only once and shared across the engine, reducing memory usage and load times.
 *
 * Supported textures and shaders.
 *
 * @note This is a singleton; use GetInstance() to access it.
 * @see Texture2D, Shader
 */
class ResourceManager{
public:

    /**
     * @brief Get the singleton instance of ResourceManager.
     * @return Reference to the ResourceManager instance.
     */
    static ResourceManager& GetInstance();

    /**
     * @brief Loads a texture from a file path, with caching.
     *
     * If the texture has already been loaded, returns the cached instance.
     * Otherwise, loads the texture from the file system and stores it in the cache.
     *
     * @param filepath Path to the texture file (e.g., "assets/textures/player.png").
     * @return Shared pointer to the loaded Texture2D, or nullptr on failure.
     */
    std::shared_ptr<Texture2D> LoadTexture(const std::string& filepath);

    /**
     * @brief Loads or retrieves a shader program by name.
     *
     * If the shader has already been compiled and cached under the given name,
     * returns the cached instance. Otherwise, compiles the shader from source
     * strings and stores it in the cache.
     *
     * @param name Unique identifier for this shader (e.g., "sprite", "ui").
     * @param vertexSource GLSL source code for the vertex shader.
     * @param fragmentSource GLSL source code for the fragment shader.
     * @return Shared pointer to the loaded Shader, or nullptr on compilation failure.
     */
    std::shared_ptr<Shader> LoadShader(const std::string& name,
                                       const std::string& vertexSource, const std::string& fragmentSource);

    /**
     * @brief Clears all cached resources.
     *
     * Releases all textures and shaders from the cache. This should be called
     * during application shutdown or when switching to a completely different
     * game state that requires different assets.
     */
    void Clear();

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    //Cache
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_Textures;
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
};
