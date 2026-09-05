#include "diagnostics/Log.h"
#include "graphics/GraphicsContext.h"
#include <glad/glad.h>
#include <format>

GraphicsContext::GraphicsContext() = default;
GraphicsContext::~GraphicsContext(){ Shutdown(); }

bool GraphicsContext::Initialize(){
    if (!CreateBuffers()) return false;
    if (!CompileShaders()) return false;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    Log::Info("Graphics context initialized.");
    return true;
}

void GraphicsContext::Shutdown(){
    m_Shader.reset();
    m_VertexArray.reset();
    m_VertexBuffer.reset();
    m_IndexBuffer.reset();
}

bool GraphicsContext::CreateBuffers(){
    float vertices[] ={
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };
    uint32_t indices[] ={ 0, 1, 2, 2, 3, 0 };

    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    m_VertexArray = std::make_unique<VertexArray>();
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

    if(!m_VertexBuffer || !m_VertexArray || !m_IndexBuffer){
        Log::Error("Failed to create graphical buffers.");
        return false;
    }

    m_VertexArray->Bind();
    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();

    return true;
}

bool GraphicsContext::CompileShaders(){
    const std::string vertexShaderSource = R"(
    #version 460 core

    layout(location = 0) in vec3 a_Position;
    layout(location = 1) in vec2 a_TexCoord;

    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;
    uniform float u_TileScale;

    out vec2 v_TexCoord;

    void main() {
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
        //gl_Position = vec4(a_Position, 1.0);
        v_TexCoord = a_TexCoord * u_TileScale;
    })";

    //Uncomment to check if ShaderSource cache is updated
    //Log::Info("Vertex Shader Source:\n" + vertexShaderSource);

    const std::string fragmentShaderSource = R"(
    #version 460 core

    in vec2 v_TexCoord;

    out vec4 FragColor;

    uniform sampler2D u_Texture;

    void main() {
        FragColor = texture(u_Texture, v_TexCoord);
    })";

    //Uncomment to check if ShaderSource cache is updated
    //Log::Info("Fragment Shader Source:\n" + fragmentShaderSource);

    m_Shader = std::make_unique<Shader>();
    if(!m_Shader->Compile(vertexShaderSource, fragmentShaderSource)){
        Log::Error("Shader compilation failed.");
        return false;
    }

    return true;
}
