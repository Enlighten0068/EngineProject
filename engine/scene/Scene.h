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

class Scene{
public:
    Scene(Shader& shader, VertexArray& va, IndexBuffer& ib);
    ~Scene();

    entt::entity CreateSpriteEntity(const Vector3D& position, const Vector3D& scale,
                                    std::shared_ptr<Texture2D> texture);

    void Update(float deltaTime);
    void Render(const Matrix4& view, const Matrix4& projection);

    entt::registry& GetRegistry() { return m_Registry; }

private:
    entt::registry m_Registry;
    Shader& m_Shader;
    VertexArray& m_VertexArray;
    IndexBuffer& m_IndexBuffer;
};
