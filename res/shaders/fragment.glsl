#version 410 core

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 emissiveColor;
uniform float alpha;

out vec4 fragColor;

void main(){
    vec3 linearColor = ambientColor * diffuseColor + emissiveColor;
    vec3 gammaCorrected = pow(linearColor, vec3(1.0/2.2));
    fragColor = vec4(gammaCorrected, alpha);
}