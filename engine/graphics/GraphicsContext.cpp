#include "diagnostics/Log.h"
#include "graphics/GraphicsContext.h"
#include <glad/glad.h>
#include <format>

GraphicsContext::GraphicsContext() = default;
GraphicsContext::~GraphicsContext(){ Shutdown(); }

/**
 * @brief Initializes the graphics context.
 *
 * Creates the vertex/index buffers, compiles the shaders, and sets up
 * the vertex attribute layout.
 *
 * @return true if all resources were created successfully, false otherwise.
 */
bool GraphicsContext::Initialize(){
    if (!CreateBuffers()) return false;
    if (!CompileShaders()) return false;

    //Define an array of vertex attribute data, see OpengGL documentation
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    Log::Info("Graphics context initialized.");
    return true;
}

/**
 * @brief Shuts down the graphics context and releases resources.
 */
void GraphicsContext::Shutdown(){
    m_Shader.reset();
    m_VertexArray.reset();
    m_VertexBuffer.reset();
    m_IndexBuffer.reset();
}

/**
 * @brief Creates vertex and index buffers for a unit quad.
 * @return true if buffers were created successfully, false otherwise.
 */
bool GraphicsContext::CreateBuffers(){
    float vertices[] ={
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //Bottom-left vertex
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //Bottom-right vertex
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f, //Top-right vertex
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f //Top-Left vertex
    };

    //Triangles
    uint32_t indices[] ={ 0, 1, 2,  //Bottom-left, bottom-right, top-right
                          2, 3, 0 }; //Top-right, top-left, bottom-left

    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    m_VertexArray = std::make_unique<VertexArray>();
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

    if(!m_VertexBuffer || !m_VertexArray || !m_IndexBuffer){
        Log::Error("Failed to create graphical buffers.");
        return false;
    }

    //Buffer binding
    m_VertexArray->Bind();
    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();

    return true;
}

/**
 * @brief Compiles the vertex and fragment shaders.
 *
 * The vertex shader transforms vertices using model, view, and projection matrices.
 * It also passes texture coordinates (scaled by u_TileScale) to the fragment shader.
 * The fragment shader samples a texture at the given coordinates.
 *
 * @return true if shaders compiled and linked successfully, false otherwise.
 */
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
