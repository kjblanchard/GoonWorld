#version 330 core
layout(location = 0) in vec2 position; // <vec2 position, vec2 texCoords>
layout(location = 1) in vec2 texturePos; // <vec2 position, vec2 texCoords>
layout(location = 2) in vec4 color; // xywh for texture location within the texture.
layout(location = 3) in vec4 textureSubRect; // xywh for texture location within the texture.
layout(location = 4) in float textureId; // xywh for texture location within the texture.

out vec2 TexCoords;
out vec4 spriteColor;
out float textureid;

// Model transform (This is handled in the vertices for these so is disabled)
// uniform mat4 model;
// Camera view
uniform mat4 view;
// World projection
uniform mat4 projection;

void main() {

    vec2 texOffset = textureSubRect.xy;
    vec2 texSize = textureSubRect.zw;
    vec2 adjustedTexCoords = texturePos * texSize + texOffset;
    TexCoords = adjustedTexCoords;
    spriteColor = color;
    textureid = textureId;
    gl_Position = projection * view * vec4(position, 0.0, 1.0);
}
