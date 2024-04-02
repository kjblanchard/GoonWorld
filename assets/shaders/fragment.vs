#version 330 core
in vec2 TexCoords;
in float a_imageNum;
out vec4 color;

uniform sampler2D image;
uniform sampler2D[16] images;
uniform vec3 spriteColor;

void main() {

    // color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
    color = vec4(spriteColor, 1.0) * texture(images[int(a_imageNum)], TexCoords);
    // color = vec4(spriteColor, 1.0) * texture(images[0], TexCoords);
}