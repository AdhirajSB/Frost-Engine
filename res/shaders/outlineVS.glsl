#version 410 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    float scale = 1.02;

    // Manual scaling matrix
    mat4 scaleMat = mat4(
        vec4(scale, 0.0,   0.0,   0.0),
        vec4(0.0,   scale, 0.0,   0.0),
        vec4(0.0,   0.0,   scale, 0.0),
        vec4(0.0,   0.0,   0.0,   1.0)
    );
    gl_Position = projection * view * model * scaleMat * vec4(aPos, 1.0);
}