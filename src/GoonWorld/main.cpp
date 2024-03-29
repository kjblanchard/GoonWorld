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
static unsigned int EBO = 0;
static geShader *shader;
static geTexture2D *texture;
extern unsigned int USE_GL_ES;

static void Update(double timeMs)
{
    // game->Update(timeMs);
}

static void Draw()
{
    // game->Draw();

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
        glActiveTexture(GL_TEXTURE0);
        geTexture2DBind(texture);
        glBindVertexArray(VAO);
    }
    // Draw the triangle
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // Draw the elements (from ebo)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // Unbind the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void InitSprite()
{
    // Triangle
    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.0f, 0.5f, 0.0f};
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    // We are using a quad, so lets use multiple indices.
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    if (!USE_GL_ES)
    {
        // create VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }
    // Make a opengl buffer to store the vertices in gpu
    glGenBuffers(1, &VBO);
    // Element buffer, for indices
    glGenBuffers(1, &EBO);
    // Tell opengl what kind of buffer this is
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copies the data from vertices into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Copies the data from indeces to the buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    if (!USE_GL_ES)
    {
        // Load the attribute pointers into the VAO
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Old, when we were just using one
        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glBindVertexArray(0);
    }
    // Unbind after we added data to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Create shader
    shader = geShaderNew();
    // We use the same shader for everything currently.
    const char *vertexShaderFile = USE_GL_ES ? "assets/shaders/vertex_es.vs" : "assets/shaders/vertex.vs";
    const char *fragmentShaderFile = USE_GL_ES ? "assets/shaders/fragment_es.vs" : "assets/shaders/fragment.vs";

    texture = geTexture2DNew();
    geTexture2DGenerate(texture, "assets/img/blocks.png");
    // geTexture2DGenerate(texture, "assets/img/container.jpg");
    geShaderCompile(shader, vertexShaderFile, fragmentShaderFile, NULL);
    geShaderSetInteger(shader, "ourTexture", 0, true);
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