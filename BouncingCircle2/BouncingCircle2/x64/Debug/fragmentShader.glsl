#version 330 core
in vec2 fragPosition;
uniform vec2 circleCenter;
out vec4 outColor;

void main() {
    float dist = length(fragPosition - circleCenter);
    float radius = 50.0 / 300.0; // Normaliz�lt sug�r
    if (dist > radius) discard; // K�r�n k�v�li pontok eldob�sa

    vec4 red = vec4(1.0, 0.0, 0.0, 1.0);   // Piros centrum
    vec4 green = vec4(0.0, 1.0, 0.0, 1.0); // Z�ld hat�r
    float t = dist / radius; // 0 (centrum) - 1 (hat�r)

    // Sz�ncsere logika
    if (gl_FrontFacing && t > 0.0) { // Csak a k�rre alkalmazzuk
        outColor = mix(green, red, t); // Norm�l esetben z�ld -> piros
        if (gl_FragCoord.z > 0.5) {    // Sz�ncsere jelz�se (pl. uniformmal is megoldhat�)
            outColor = mix(red, green, t); // Cser�lt esetben piros -> z�ld
        }
    } else {
        outColor = vec4(0.0, 0.0, 1.0, 1.0); // K�k szakasz
    }
}