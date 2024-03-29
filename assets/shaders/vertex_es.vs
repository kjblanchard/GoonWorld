attribute vec3 aPos; // Cannot use VAOs, so need attribute passed in.

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}