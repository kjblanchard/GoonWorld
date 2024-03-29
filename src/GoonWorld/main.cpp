#include <GoonWorld/core/Game.hpp>
#include <GoonEngine/test.h>
// opengl
#include <GoonEngine/Shader.h>
#include <GoonEngine/Sprite.h>
#include <GoonEngine/Texture2D.h>

using namespace GoonWorld;
static std::unique_ptr<Game> game;
static unsigned int VBO = 0;
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

    // Use the shader program
    geShaderUse(shader);

    // Bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Specify the vertex attribute pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

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
    // Make a opengl buffer to store the vertices in gpu
    glGenBuffers(1, &VBO);
    // Tell opengl what kind of buffer this is
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copies the data from vertices into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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