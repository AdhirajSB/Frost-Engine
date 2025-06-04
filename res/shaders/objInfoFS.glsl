#version 410 core

uniform uint objectID;
out uint FragColor;

void main(){
    FragColor = objectID;
}