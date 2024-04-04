#version 330 core
layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout(location = 1) in float imageNum;
layout(location = 2) in vec4 color;
layout(location = 3) in vec4 textureSourceRect;

out vec2 TexCoords;
out float a_imageNum;
out vec4 a_color;

// Camera view
uniform mat4 view;
// World view
uniform mat4 projection;

void main() {
    vec2 texOffset = textureSourceRect.xy;
    vec2 texSize = textureSourceRect.zw;
    vec2 adjustedTexCoords = vertex.zw * texSize + texOffset;
    TexCoords = adjustedTexCoords;
    a_imageNum = imageNum;
    a_color = color;
    // gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);
    vec2 scaledVertex = vertex.xy * 2.0;
    // gl_Position = projection * view * vec4(vertex.xy, 0.0, 1.0);
    gl_Position = projection * view * vec4(scaledVertex, 0.0, 1.0);
}
