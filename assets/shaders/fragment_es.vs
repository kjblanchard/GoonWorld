precision mediump float; // Specify precision for float

varying vec3 ourColor;
varying vec2 TexCoord;

uniform sampler2D ourTexture;

void main() {
    gl_FragColor = texture2D(ourTexture, TexCoord);
    // gl_FragColor = vec4(ourColor, 1.0);
}