#version 330
out vec4 fragColor;
uniform vec4 drawColor;
void main() {
    fragColor = drawColor;
}