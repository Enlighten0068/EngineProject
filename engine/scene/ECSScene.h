#pragma once

#include "components/Transform.h"
#include "components/SpriteRenderer.h"
#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"
#include "graphics/Texture2D.h"
#include "math/Vector3D.h"
#include <entt.hpp>
#include <memory>

/**
 * @brief Scene class that manages an ECS registry with sprite entities.
 *
 * ECSScene provides a high-level interface for creating and rendering
 * sprite entities using the ECS pattern. It holds the registry, shader,
 * and graphics buffers needed for rendering.
 *
 * @see RenderSystem, entt::registry
 */
class ECSScene{
public:
    /**
     * @brief Constructs an ECSScene.
     * @param shader Reference to the shader for rendering.
     * @param va Reference to the vertex array.
     * @param ib Reference to the index buffer.
     */
    ECSScene(Shader& shader, VertexArray& va, IndexBuffer& ib);
    ~ECSScene();

    /**
     * @brief Creates a sprite entity with Transform and SpriteRenderer components.
     * @param position Initial world position.
     * @param scale Initial scale (width and height).
     * @param texture Texture to render for this sprite.
     * @return Entity ID of the created entity.
     */
    entt::entity CreateSpriteEntity(const Vector3D& position, const Vector3D& scale,
                                    std::shared_ptr<Texture2D> texture);

    /**
     * @brief Renders all sprite entities in the registry.
     * @param view View matrix from the camera.
     * @param projection Projection matrix from the camera.
     */
    void Render(const Matrix4& view, const Matrix4& projection);

    /**
     * @brief Gets the ECS registry.
     * @return Reference to the registry.
     */
    entt::registry& GetRegistry(){ return m_Registry; }

private:
    entt::registry m_Registry; //ECS registry
    Shader& m_Shader; //Shader for rendering
    VertexArray& m_VertexArray; //Vertex array for geometry
    IndexBuffer& m_IndexBuffer; //Index buffer for geometry
};
