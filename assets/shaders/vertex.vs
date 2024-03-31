#version 330 core
layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

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
    // gl_Position = projection  * model * vec4(vertex.xy, 0.0, 1.0);
    gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);
}
