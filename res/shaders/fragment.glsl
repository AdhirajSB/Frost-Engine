#version 410 core

in vec3 Pos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Tangent;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 emissiveColor;
uniform float alpha;
uniform float specularExponent;

uniform sampler2D diffuseTexture;
uniform sampler2D alphaTexture;
uniform sampler2D normalTexture;

uniform bool hasDiffuseTexture;
uniform bool hasAlphaTexture;
uniform bool hasNormalTexture;

uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 cameraPos;

uniform float constant = 1.0;
uniform float linear = 0.14;
uniform float quadratic = 0.07;

out vec4 fragColor;

void main(){
    // Final alpha
    float effectiveAlpha = alpha;
    if (hasAlphaTexture) {
        effectiveAlpha *= texture(alphaTexture, TexCoords).a;
    }
    if (effectiveAlpha < 0.1) discard;

    // Base color
    vec3 effectiveDiffuse = hasDiffuseTexture ? texture(diffuseTexture, TexCoords).rgb : diffuseColor;

    // === TBN matrix ===
    vec3 T = normalize(Tangent);
    vec3 N = normalize(Normal);
    vec3 B = normalize(cross(N, T)); // Bitangent (T x N)

    mat3 TBN = mat3(T, B, N);

    // === Normal Mapping ===
    vec3 mappedNormal = N;
    if (hasNormalTexture) {
        vec3 normalSample = texture(normalTexture, TexCoords).rgb;
        normalSample = normalSample * 2.0 - 1.0; // [0,1] → [-1,1]
        mappedNormal = normalize(TBN * normalSample);
    }

    vec3 L = normalize(lightPos - Pos);
    vec3 V = normalize(cameraPos - Pos);
    vec3 R = reflect(-L, mappedNormal);

    // Lighting components
    float diff = max(dot(mappedNormal, L), 0.0);
    float spec = 0.0;
    float effectiveShininess = clamp((specularExponent / 1000.0) * 256.0, 1.0, 256.0);
    if (diff > 0.0) {
        spec = pow(max(dot(V, R), 0.0), effectiveShininess);
    }

    float distance = length(lightPos - Pos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);

    vec3 ambient = 0.1 * ambientColor * effectiveDiffuse;
    vec3 diffuse = lightCol * effectiveDiffuse * diff * attenuation;
    vec3 specular = lightCol * specularColor * spec * attenuation;

    vec3 linearColor = ambient + diffuse + specular + emissiveColor;
    fragColor = vec4(linearColor, effectiveAlpha);
}
