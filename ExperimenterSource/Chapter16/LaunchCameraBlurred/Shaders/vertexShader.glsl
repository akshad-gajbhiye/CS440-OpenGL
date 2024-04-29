#version 430 core

layout(location=0) in vec4 launchCoords;
layout(location=1) in vec2 launchTexCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;

out vec2 texCoordsExport;

void main(void)
{   
   texCoordsExport = launchTexCoords;
   gl_Position = projMat * modelViewMat * launchCoords;
}
