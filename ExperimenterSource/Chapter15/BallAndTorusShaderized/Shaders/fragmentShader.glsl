#version 430 core

uniform vec4 colors;

out vec4 colorsOut;

void main(void)
{
   colorsOut = colors; 
}