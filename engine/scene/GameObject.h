/
#pragma once

#include "math/Transform.h"
#include "graphics/Texture2D.h"

class SpriteRenderer;

/**
 * @brief Legacy GameObject class (not currently used).
 *
 * @deprecated This class has been superseded by the ECS system.
 * @see ECSScene, Components::Transform, Components::SpriteRenderer
 */
class GameObject{
public:
    GameObject();
    ~GameObject();

    void SetTexture(const std::shared_ptr<Texture2D>& texture);
    Transform& GetTransform();
    void Update(float deltaTime);
    void Render(Shader& shader, const Matrix4& view, const Matrix4& projection);

private:
    Transform m_Transform; //Transform component
    std::shared_ptr<Texture2D> m_Texture; //Texture
};
