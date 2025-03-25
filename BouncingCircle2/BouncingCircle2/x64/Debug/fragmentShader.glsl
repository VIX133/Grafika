#version 330 core
in vec2 fragPosition;
uniform vec2 circleCenter;
out vec4 outColor;

void main() {
    float dist = length(fragPosition - circleCenter);
    float radius = 50.0 / 300.0; // Normalizált sugár
    if (dist > radius) discard; // Körön kívüli pontok eldobása

    vec4 red = vec4(1.0, 0.0, 0.0, 1.0);   // Piros centrum
    vec4 green = vec4(0.0, 1.0, 0.0, 1.0); // Zöld határ
    float t = dist / radius; // 0 (centrum) - 1 (határ)

    // Színcsere logika
    if (gl_FrontFacing && t > 0.0) { // Csak a körre alkalmazzuk
        outColor = mix(green, red, t); // Normál esetben zöld -> piros
        if (gl_FragCoord.z > 0.5) {    // Színcsere jelzése (pl. uniformmal is megoldható)
            outColor = mix(red, green, t); // Cserélt esetben piros -> zöld
        }
    } else {
        outColor = vec4(0.0, 0.0, 1.0, 1.0); // Kék szakasz
    }
}