#version 410 core

uniform uint objectID;
out uvec3 FragColor;

void main(){
    FragColor = uvec3(objectID, 0, 0);
}