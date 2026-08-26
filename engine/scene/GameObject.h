#pragma once

#include "math/Transform.h"
#include "graphics/Texture2D.h"

class SpriteRenderer;

class GameObject {
public:
    GameObject();
    ~GameObject();

    void SetTexture(const std::shared_ptr<Texture2D>& texture);
    Transform& GetTransform();
    void Update(float deltaTime);
    void Render(Shader& shader, const Matrix4& view, const Matrix4& projection);

private:
    Transform m_Transform;
    std::shared_ptr<Texture2D> m_Texture;
};
