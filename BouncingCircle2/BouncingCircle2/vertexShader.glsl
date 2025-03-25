#version 330 core
layout(location = 0) in vec2 vertexPosition;
uniform mat4 matProjection;
uniform mat4 matModelView;
uniform float offsetX;
uniform float offsetY;
out vec2 fragPosition;

void main() {
    vec2 pos = vertexPosition + vec2(offsetX, offsetY);
    gl_Position = matProjection * matModelView * vec4(pos, 0.0, 1.0);
    fragPosition = vertexPosition; // A kör helyi koordinátáit adjuk tovább
}