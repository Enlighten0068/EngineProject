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

class RenderSystem {
public:
    static void Render(entt::registry& registry, Shader& shader, const Matrix4& view,
                       const Matrix4& projection, VertexArray& va, IndexBuffer& ib) {



        auto view_entities = registry.view<Components::Transform, Components::SpriteRenderer>();
        //Log::Info(std::format("RenderSystem: {} entities to render", view_entities.size()));

        for (auto [entity, transform, sprite] : view_entities.each()) {
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
