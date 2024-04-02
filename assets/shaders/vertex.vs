#version 330 core
layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout(location = 1) in float imageNum;
layout(location = 2) in vec4 color;

out vec2 TexCoords;
out float a_imageNum;
out vec4 a_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 texOffset;
uniform vec2 texSize;
uniform bool flipHorizontal;

void main() {
    vec2 adjustedTexCoords = vertex.zw * texSize + texOffset;
    if(flipHorizontal) {
        // Calculate the original normalized texture coordinates within the subtexture
        vec2 originalCoords = (adjustedTexCoords - texOffset) / texSize;
        // Mirror the texture coordinates horizontally within the subtexture
        originalCoords.x = 1.0 - originalCoords.x;
        // Transform back to the texture space
        adjustedTexCoords = originalCoords * texSize + texOffset;
    }
    TexCoords = adjustedTexCoords;
    a_imageNum = imageNum;
    a_color = color;
    gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);
}
