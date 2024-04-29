#version 430 core

layout(location=0) in vec4 coords;
layout(location=1) in vec2 texCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;

out vec2 texCoordsExport;

void main(void)
{   
   gl_Position = projMat * modelViewMat * coords;
   texCoordsExport = texCoords;
}
