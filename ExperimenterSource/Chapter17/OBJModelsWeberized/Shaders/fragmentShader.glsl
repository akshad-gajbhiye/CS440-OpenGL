#version 300 es
precision mediump float;

uniform vec4 colors;

out vec4 colorsOut;

void main() 
{
  colorsOut = colors;
}