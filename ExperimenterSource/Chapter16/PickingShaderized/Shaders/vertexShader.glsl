#version 430 core

layout(location=0) in vec4 coords;
layout(location=1) in vec4 colors;

uniform mat4 modelViewMat;
uniform mat4 projMat;

out vec4 colorsExport;

void main(void)
{
   gl_Position = projMat * modelViewMat * coords;
   colorsExport = colors;
}