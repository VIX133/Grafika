#version 330 core
layout(location = 0) in vec2 inPosition;
uniform vec2 circleCenter;
uniform mat4 matModelView;
uniform mat4 matProjection;

out vec2 fragPosition;

void main() {
    vec2 pos = inPosition + circleCenter;
    gl_Position = matProjection * matModelView * vec4(pos, 0.0, 1.0);
    fragPosition = pos;
}