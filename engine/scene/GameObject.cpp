#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/IndexBuffer.h"
#include "renderer/Renderer.h"
#include "scene/GameObject.h"

GameObject::GameObject() : m_Texture(nullptr){}

GameObject::~GameObject() = default;

void GameObject::SetTexture(const std::shared_ptr<Texture2D>& texture){
    m_Texture = texture;
}

Transform& GameObject::GetTransform(){
    return m_Transform;
}

void GameObject::Update(float deltaTime){
    //Legacy method - not used
}

void GameObject::Render(Shader& shader, const Matrix4& view, const Matrix4& projection){
    if(!m_Texture) return;

    Matrix4 model = Matrix4::Translation(m_Transform.Position);
    model = model * Matrix4::RotationZ(m_Transform.Rotation.z);
    model = model * Matrix4::Scale(m_Transform.Scale);

    //Rendering would go here using Renderer::DrawTexturedQuad
    //But this class is deprecated in favor of ECSScene
}
