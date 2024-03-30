#include <GoonWorld/core/Game.hpp>
#include <GoonEngine/test.h>
// opengl
#include <GoonEngine/Shader.h>
#include <GoonEngine/Sprite.h>
#include <GoonEngine/Texture2D.h>
#include <cglm/mat4.h>
#include <cglm/cglm.h>
#include <GoonWorld/models/AppSettings.hpp>

using namespace GoonWorld;
static std::unique_ptr<Game> game;
static unsigned int VBO = 0;
static unsigned int VAO = 0;
static unsigned int EBO = 0;
geShader *shader;
static geTexture2D *texture;
static geSpriteRenderer *sprite;
extern unsigned int USE_GL_ES;

static int ticks = 0;

static void InitSprite()
{
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    // We are using a quad, so lets use multiple indices. to draw multiple triangle with the same indices
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    if (!USE_GL_ES)
    {
        // create VAO and bind to it, which holds the pointers for vertex attributes
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }
    // Make a opengl buffer to store the vertices, color, map, etc in gpu
    glGenBuffers(1, &VBO);
    // Element buffer, for indices
    glGenBuffers(1, &EBO);
    // Tell opengl what kind of buffer this is
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copies the data from vertices into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Copies the data from indices to the buffer
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
    geShaderCompile(shader, vertexShaderFile, fragmentShaderFile, NULL);
    geShaderSetInteger(shader, "ourTexture", 0, true);
}

static void DrawSimple()
{
    geShaderUse(shader);
    if (USE_GL_ES)
    {
        // Bind the vertex buffer
        glActiveTexture(GL_TEXTURE0);
        geTexture2DBind(texture);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // Specify the vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
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
static void UpdateSimple()
{
    // Scale and rotate
    ++ticks;
    mat4 trans;
    glm_mat4_identity(trans);
    auto rotationRadians = glm_rad(ticks);
    // glm_rotate(trans, rotationRadians, vec3{0, 0, 1.0});
    auto rotationRadiansz = glm_rad(-ticks);
    // glm_rotate(trans, rotationRadiansz, vec3{0, 1.0, 0});
    // glm_scale(trans, vec3{1, 1, 1});
    geShaderSetMatrix4(shader, "transform", &trans, true);
}

static void InitReal(int width, int height)
{
    shader = geShaderNew();
    const char *vertexShaderFile = USE_GL_ES ? "assets/shaders/vertex_es.vs" : "assets/shaders/vertex.vs";
    const char *fragmentShaderFile = USE_GL_ES ? "assets/shaders/fragment_es.vs" : "assets/shaders/fragment.vs";

    // sprite = geSpriteRendererNew(shader);
    // texture = geTexture2DNew();
    // geTexture2DGenerate(texture, "assets/img/blocks.png");
    geShaderCompile(shader, vertexShaderFile, fragmentShaderFile, NULL);
    // geShaderSetInteger(shader, "ourTexture", 0, true);
    mat4 projection;
    glm_mat4_identity(projection);
    glm_ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f, projection);
    geShaderSetInteger(shader, "image", 0, true);
    geShaderSetMatrix4(shader, "projection", &projection, true);
}

static void Update(double timeMs)
{
    game->Update(timeMs);
}

static void Draw()
{
    game->Draw();
    // geSpriteRendererDraw(sprite, texture, vec2{128, 256}, vec2{16, 16}, 0, vec3{1, 1, 1}, vec2{32,32}, vec2{16,16});
}

int main()
{
    geInitializeEngine();
    geGameSetUpdateFunc(Update);
    geGameSetDrawFunc(Draw);
    game = std::make_unique<Game>();
    InitReal(game->GetAppSettings().WindowConfig.WindowSize.x, game->GetAppSettings().WindowConfig.WindowSize.y);
    std::string levelName = "level1";
    game->StartGameLevel(levelName);
    gePlayLoop();
}