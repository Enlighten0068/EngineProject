#pragma once

#include "components/Transform.h"
#include "components/SpriteRenderer.h"
#include "components/TileScale.h"
#include "diagnostics/Log.h"
#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"
#include "math/Matrix4.h"
#include "renderer/Renderer.h"
#include <entt.hpp>

/**
 * @brief System that renders all entities with Transform and SpriteRenderer components.
 *
 * The RenderSystem is responsible for drawing all visible entities in the scene.
 * It processes entities that have both a Transform (position, rotation, scale)
 * and a SpriteRenderer (texture) component, calculates the model matrix, and
 * calls the Renderer to draw each entity.
 *
 * Entities with a TileScale component use its value; entities without TileScale
 * default to 1.0f (no repetition, texture stretches to fill the quad).
 *
 * @note This system should be called once per frame from the scene's Render() method.
 * @see Components::Transform, Components::SpriteRenderer, Components::TileScale, Renderer
 */
class RenderSystem{
public:
    /**
     * @brief Renders all entities with Transform and SpriteRenderer components.
     *
     * This method iterates over all entities that have both Transform and SpriteRenderer
     * components. For each entity, it checks if the texture is valid, builds the model
     * matrix, and calls DrawTexturedQuad on the Renderer.
     *
     * @param registry Reference to the ECS registry containing all entities.
     * @param shader The shader program to use for rendering.
     * @param view The view matrix (camera position).
     * @param projection The projection matrix (orthographic or perspective).
     * @param va The vertex array object containing geometry data.
     * @param ib The index buffer containing the quad indices.
     */
    static void Render(entt::registry& registry, Shader& shader, const Matrix4& view,
                       const Matrix4& projection, VertexArray& va, IndexBuffer& ib){
        auto view_entities = registry.view<Components::Transform, Components::SpriteRenderer>();

        for(auto [entity, transform, sprite] : view_entities.each()){
            //Skip entities without texture
            if(!sprite.Texture) continue;

            //Build model matrix: translation * rotation * scale
            Matrix4 model = Matrix4::Translation(transform.Position);
            model = model * Matrix4::RotationZ(transform.Rotation.z);
            model = model * Matrix4::Scale(transform.Scale);

            //Get tile scale (default to 1.0 if not present)
            float tileScaleX = 1.0f;
            float tileScaleY = 1.0f;
            if(registry.all_of<Components::TileScale>(entity)){
                const auto& ts = registry.get<Components::TileScale>(entity);
                tileScaleX = ts.ScaleX;
                tileScaleY = ts.ScaleY;
            }

            //Draw the textured quad
            Renderer::DrawTexturedQuad(shader, va, ib, *sprite.Texture,
                                       model, view, projection, tileScaleX, tileScaleY);
        }
    }
};
