#include "core/Input.h"
#include "diagnostics/Log.h"
#include "renderer/Renderer.h"
#include "scene/ECSScene.h"
#include "systems/RenderSystem.h"

ECSScene::ECSScene(Shader& shader, VertexArray& va, IndexBuffer& ib)
: m_Shader(shader), m_VertexArray(va), m_IndexBuffer(ib){}

ECSScene::~ECSScene() = default;

/**
 * @brief Creates a sprite entity with Transform and SpriteRenderer components.
 * @param position Initial world position.
 * @param scale Initial scale (width and height).
 * @param texture Texture to render for this sprite.
 * @return Entity ID of the created entity.
 */
entt::entity ECSScene::CreateSpriteEntity(const Vector3D& position, const Vector3D& scale,
                                          std::shared_ptr<Texture2D> texture){
    auto entity = m_Registry.create();
    auto& transform = m_Registry.emplace<Components::Transform>(entity);
    transform.Position = position;
    transform.Scale = scale;
    auto& sprite = m_Registry.emplace<Components::SpriteRenderer>(entity);
    sprite.Texture = texture;
    return entity;
}

/**
* @brief Renders all sprite entities in the registry.
* @param view View matrix from the camera.
* @param projection Projection matrix from the camera.
*/
void ECSScene::Render(const Matrix4& view, const Matrix4& projection){
    RenderSystem::Render(m_Registry, m_Shader, view, projection, m_VertexArray, m_IndexBuffer);
}
