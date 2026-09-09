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
    if(!CreateBuffers()) return false;
    if(!CompileShaders()) return false;
    if(!CompileLineShader()) return false;

    //Define vertex attribute layout: position (3 floats) + texture coordinates (2 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Enable alpha blending for texture transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Log::Info("Graphics context initialized.");
    return true;
}

/**
 * @brief Shuts down the graphics context and releases resources.
 */
void GraphicsContext::Shutdown(){
    m_Shader.reset();
    m_LineShader.reset();
    m_VertexArray.reset();
    m_VertexBuffer.reset();
    m_IndexBuffer.reset();
}

/**
 * @brief Creates vertex and index buffers for a unit quad.
 * @return true if buffers were created successfully, false otherwise.
 */
bool GraphicsContext::CreateBuffers(){
    //Quad vertices: position (x,y,z) and texture coordinates (u,v)
    float vertices[] ={
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //Bottom-left
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //Bottom-right
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f, //Top-right
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f //Top-left
    };

    //Triangle indices for the quad (two triangles)
    uint32_t indices[] ={ 0, 1, 2, //Triangle 1
        2, 3, 0 };//Triangle 2

        m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
        m_VertexArray = std::make_unique<VertexArray>();
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        if(!m_VertexBuffer || !m_VertexArray || !m_IndexBuffer){
            Log::Error("Failed to create graphical buffers.");
            return false;
        }

        //Bind all buffers
        m_VertexArray->Bind();
        m_VertexBuffer->Bind();
        m_IndexBuffer->Bind();

        return true;
}

/**
 * @brief Compiles the vertex and fragment shaders for sprite rendering.
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
    uniform vec2 u_TileScale;

    out vec2 v_TexCoord;

    void main(){
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
        v_TexCoord = a_TexCoord * u_TileScale;
    })";

    const std::string fragmentShaderSource = R"(
    #version 460 core

    in vec2 v_TexCoord;

    out vec4 FragColor;

    uniform sampler2D u_Texture;

    void main(){
        FragColor = texture(u_Texture, v_TexCoord);
    })";

    m_Shader = std::make_unique<Shader>();
    if(!m_Shader->Compile(vertexShaderSource, fragmentShaderSource)){
        Log::Error("Shader compilation failed.");
        return false;
    }

    return true;
}

/**
 * @brief Compiles the vertex and fragment shaders for line rendering.
 * @return true if shaders compiled and linked successfully, false otherwise.
 */
bool GraphicsContext::CompileLineShader(){
    const std::string vertexSource = R"(
    #version 460 core
    layout(location = 0) in vec3 a_Position;
    uniform mat4 u_Model;
    uniform mat4 u_View;
    uniform mat4 u_Projection;
    void main(){
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
    })";

    const std::string fragmentSource = R"(
    #version 460 core
    out vec4 FragColor;
    uniform vec3 u_Color;
    void main(){
        FragColor = vec4(u_Color, 1.0);
    })";

    m_LineShader = std::make_unique<Shader>();
    if(!m_LineShader->Compile(vertexSource, fragmentSource)){
        Log::Error("Line shader compilation failed.");
        return false;
    }

    Log::Info("Line shader compiled successfully.");
    return true;
}
