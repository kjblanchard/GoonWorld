attribute vec3 aPos; // Cannot use VAOs, so need attribute passed in.
attribute vec3 aColor;
attribute vec2 aTexCoord;

varying vec3 ourColor;
varying vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}