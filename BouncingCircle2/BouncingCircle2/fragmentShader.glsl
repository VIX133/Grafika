#version 330 core
in vec2 fragPosition;
uniform vec2 center;
uniform float radius;
uniform float lineY;
out vec4 fragColor;

void main() {
    float dist = distance(fragPosition, vec2(0.0, 0.0));
    if (dist <= radius) {
        float t = dist / radius;
        vec3 red = vec3(1.0, 0.0, 0.0); 
        vec3 green = vec3(0.0, 1.0, 0.0); 
        float lineHalfLength = 1.0 / 2.5;
        
        bool intersects = (abs(center.y - lineY) <= radius) && (abs(center.x) < lineHalfLength);
        vec3 color;
        if (radius > 0.5) {
            color = vec3(0.0, 0.0, 1.0);
        } else if (!intersects) {
            color = mix(green, red, t);
        } else { 
            color = mix(red, green, t);
        }
        fragColor = vec4(color, 1.0);
    } else {
        discard;
    }
}