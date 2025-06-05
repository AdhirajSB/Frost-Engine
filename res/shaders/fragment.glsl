#version 410 core

in vec2 TexCoords;

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 emissiveColor;
uniform float alpha;

uniform sampler2D diffuseTexture;
uniform sampler2D alphaTexture;
uniform sampler2D normalTexture;

uniform bool hasDiffuseTexture;
uniform bool hasAlphaTexture;
uniform bool hasNormalTexture;

out vec4 fragColor;

void main(){
    vec3 linearColor = ambientColor * diffuseColor + emissiveColor;

    if (hasDiffuseTexture){
        linearColor = texture(diffuseTexture, TexCoords).rgb;
    }

    vec3 gammaCorrected = pow(linearColor, vec3(1.0/2.2));
    fragColor = vec4(gammaCorrected, alpha);
}