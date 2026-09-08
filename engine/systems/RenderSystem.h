#pragma once

#include "components/Transform.h"
#include "components/SpriteRenderer.h"
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
 * @note This system should be called once per frame from the scene's Render() method.
 * @see Transform, SpriteRenderer, Renderer
 */
class RenderSystem{
public:

    /**
     * @brief Renders all entities with Transform and SpriteRenderer components.
     *
     * This method iterates over all entities that have both Transform and SpriteRenderer
     * components. For each entity checks if texture is valid, build the model matrix,
     * and calls the Draw Textured Quad Function.
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
        //Log::Info(std::format("RenderSystem: {} entities to render", view_entities.size()));

        //Debugging - log the number of entities being rendered
        //Log::Info(std::format("RenderSystem: {} entities to render", view_entities.size()));

        for (auto [entity, transform, sprite] : view_entities.each()){

            //Skip entities without texture
            if (!sprite.Texture) continue;

            Matrix4 model = Matrix4::Translation(transform.Position);
            model = model * Matrix4::RotationZ(transform.Rotation.z);
            model = model * Matrix4::Scale(transform.Scale);

            //Log::Info(std::format("[RenderSystem] Entity scale: ({}, {}), Rotation Z: {}",
                                  //transform.Scale.x, transform.Scale.y, transform.Rotation.z));
            //Log::Info(std::format("[RenderSystem] Model[0]={}, Model[5]={}",
                                  //model.Data()[0], model.Data()[5]));

            float tileScale = 1.0f;
            Renderer::DrawTexturedQuad(shader, va, ib, *sprite.Texture, model, view, projection, tileScale);
            }
        }
};
