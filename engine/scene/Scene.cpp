#include "core/Input.h"
#include "diagnostics/Log.h"
#include "renderer/Renderer.h"
#include "scene/Scene.h"
#include "systems/RenderSystem.h"


Scene::Scene(Shader& shader, VertexArray& va, IndexBuffer& ib)
: m_Shader(shader), m_VertexArray(va), m_IndexBuffer(ib){}

Scene::~Scene() = default;

entt::entity Scene::CreateSpriteEntity(const Vector3D& position, const Vector3D& scale,
                                       std::shared_ptr<Texture2D> texture){
    auto entity = m_Registry.create();
    auto& transform = m_Registry.emplace<Components::Transform>(entity);
    transform.Position = position;
    transform.Scale = scale;
    auto& sprite = m_Registry.emplace<Components::SpriteRenderer>(entity);
    sprite.Texture = texture;
    return entity;
}

void Scene::Update(float deltaTime){
}

void Scene::Render(const Matrix4& view, const Matrix4& projection){
    RenderSystem::Render(m_Registry, m_Shader, view, projection, m_VertexArray, m_IndexBuffer);
}

