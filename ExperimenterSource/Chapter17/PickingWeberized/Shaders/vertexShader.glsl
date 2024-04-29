#version 300 es

layout(location=0) in vec4 coords;
layout(location=1) in vec4 colors;

uniform mat4 modelViewMat;
uniform mat4 projMat;

out vec4 colorsExport;

void main() 
{
  gl_Position = projMat * modelViewMat * coords;
  colorsExport = colors;
}