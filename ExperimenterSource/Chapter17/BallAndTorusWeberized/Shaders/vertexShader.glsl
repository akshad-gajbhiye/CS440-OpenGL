#version 300 es

layout(location=0) in vec4 coords;

uniform mat4 modelViewMat;
uniform mat4 projMat;

void main() 
{
  gl_Position = projMat * modelViewMat * coords;
}