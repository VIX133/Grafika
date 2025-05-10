#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 vertexTexCoord;

out vec3 varyingColor;
out vec3 varyingNormal;
out vec3 varyingFragmentPosition;
out vec2 varyingTexCoord;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat3 inverseTransposeMatrix;

void main() {
    gl_Position = matProjection * matView * matModel * vec4(vertexPosition, 1.0);
    varyingColor = vertexColor;
    varyingNormal = inverseTransposeMatrix * vertexNormal;
    varyingFragmentPosition = vec3(matModel * vec4(vertexPosition, 1.0));
    varyingTexCoord = vertexTexCoord;
}