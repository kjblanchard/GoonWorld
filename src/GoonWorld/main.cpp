#include <GoonWorld/core/Game.hpp>
#include <GoonEngine/test.h>
// opengl
#include <GoonEngine/Shader.h>
#include <GoonEngine/Sprite.h>
#include <GoonEngine/Texture2D.h>

using namespace GoonWorld;
static std::unique_ptr<Game> game;
static unsigned int VBO = 0;
static unsigned int VAO = 0;
static geShader *shader;
extern unsigned int USE_GL_ES;

static void Update(double timeMs)
{
    // game->Update(timeMs);
}

static void Draw()
{
    // game->Draw();
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glEnableVertexAttribArray(0);
    // // Tell about attribute 0 going in
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    // Bind our shader
    geShaderUse(shader);

    if (USE_GL_ES)
    {
        // Bind the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Specify the vertex attribute pointers
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    }
    else
    {
        glBindVertexArray(VAO);
    }
    // Draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // Unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void InitSprite()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};
    if (!USE_GL_ES)
    {
        // create VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }
    // Make a opengl buffer to store the vertices in gpu
    glGenBuffers(1, &VBO);
    // Tell opengl what kind of buffer this is
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copies the data from vertices into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    if (!USE_GL_ES)
    {
        // Add attribute pointer to the vertex array object, and unbind it
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glBindVertexArray(0);
    }
    // Unbind after we added data to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Create shader
    shader = geShaderNew();
    // We use the same shader for everything currently.
    const char *vertexShaderFile = USE_GL_ES ? "assets/shaders/vertex_es.vs" : "assets/shaders/vertex.vs";
    const char *fragmentShaderFile = USE_GL_ES ? "assets/shaders/fragment_es.vs" : "assets/shaders/fragment.vs";

    geShaderCompile(shader, vertexShaderFile, fragmentShaderFile, NULL);
    geShaderUse(shader);
}

int main()
{
    geInitializeEngine();
    geGameSetUpdateFunc(Update);
    geGameSetDrawFunc(Draw);
    game = std::make_unique<Game>();
    InitSprite();
    gePlayLoop();
}