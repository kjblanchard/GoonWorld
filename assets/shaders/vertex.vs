// #version 330 core
// layout(location = 0) in vec3 aPos;
// layout(location = 1) in vec3 aColor;
// layout(location = 2) in vec2 aTexCoord;

// out vec3 ourColor;
// out vec2 TexCoord;

// uniform mat4 transform;

// void main() {
//     // gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
//     // Apply the transform by multiplying it by our position (vec3) and 1.0 (to make it a vec4)
//     gl_Position = transform * vec4(aPos, 1.0f);
//     ourColor = aColor;
//     TexCoord = aTexCoord;
// }

#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}