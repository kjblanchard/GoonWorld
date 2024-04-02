#version 330 core
in vec2 TexCoords;
in float a_imageNum;
in vec4 a_color;
out vec4 color;

uniform sampler2D image;
uniform sampler2D[16] images;

void main() {

    color = a_color * texture(images[int(a_imageNum)], TexCoords);
}