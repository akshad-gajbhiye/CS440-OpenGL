#version 430 core

layout(location=0) in vec4 torCoords;

uniform mat4 projMat;
uniform mat4 modelViewMat;

void main(void)
{   
   gl_Position = projMat * modelViewMat * torCoords;
}
