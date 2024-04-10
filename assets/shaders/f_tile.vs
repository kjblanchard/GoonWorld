#version 330 core
in vec2 TexCoords;
in float a_imageNum;
in vec4 a_color;
out vec4 color;

uniform sampler2D[16] timages;

void main() {
    // color = a_color * texture(timages[int(a_imageNum)], TexCoords);
    color = vec4(1,1,1,1)
}