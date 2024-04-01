#version 330 core
in vec2 TexCoords;
in vec4 spriteColor;
in float textureid;
out vec4 color;
uniform sampler2D u_Textures[16]; // Use uniform here

void main() {

    color = spriteColor * texture(u_Textures[int(textureid)], TexCoords);
}