#version 330 core

in vec3 varyingColor;
in vec3 varyingNormal;
in vec3 varyingFragmentPosition;
in vec2 varyingTexCoord; 

uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform bool useLighting;
uniform sampler2D textureSampler; 
uniform bool useTexture;

out vec4 outColor;

// Anyag tulajdonságok - ezeket lehetne uniform változóként átadni
vec3 ambientMaterial = vec3(0.2, 0.2, 0.2);
vec3 diffuseMaterial = varyingColor;
vec3 specularMaterial = vec3(0.8, 0.8, 0.8);

// Fény tulajdonságok
const vec3 ambientColor = vec3(1.0, 1.0, 1.0);
const float ambientStrength = 0.25;
const vec3 diffuseColor = vec3(212.0 / 255.0, 175.0 / 255.0, 55.0 / 255.0); // gold
const float diffuseStrength = 1.0;
const vec3 specularColor = vec3(1.0, 1.0, 1.0); // fehér spekuláris csúcsfény
const float specularStrength = 0.8;
const int specularShininess = 32;

void main(void) {
    vec4 textureColor = vec4(varyingColor, 1.0);
    
    // Ha használunk textúrát és van textúra sampler, akkor alkalmazzuk a textúrát
    if (useTexture) {
        textureColor = texture(textureSampler, varyingTexCoord);
    }
    
    if (useLighting) {
        // Ambiens komponens
        vec3 ambient = ambientStrength * ambientColor * ambientMaterial;
        
        // Diffúz komponens
        vec3 normalizedNormal = normalize(varyingNormal);
        vec3 lightDirection = normalize(lightPosition - varyingFragmentPosition);
        float diffuseCoEfficient = max(dot(normalizedNormal, lightDirection), 0.0);
        vec3 diffuse = diffuseStrength * diffuseCoEfficient * diffuseColor * diffuseMaterial;
        
        // Spekuláris komponens
        vec3 viewDirection = normalize(cameraPosition - varyingFragmentPosition);
        vec3 reflectDirection = reflect(-lightDirection, normalizedNormal);
        float specularCoEfficient = pow(max(dot(viewDirection, reflectDirection), 0.0), specularShininess);
        vec3 specular = specularStrength * specularCoEfficient * specularColor * specularMaterial;
        
        // Végsõ szín
        vec3 result = ambient + diffuse + specular;
        outColor = vec4(result, 1.0) * textureColor;
    } else {
        outColor = textureColor;
    }
}