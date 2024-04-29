#version 300 es
precision mediump float;

in vec2 texCoordsExport;

uniform sampler2D sugaryTex;

out vec4 colorsOut;

void main() 
{
  colorsOut = texture(sugaryTex, texCoordsExport);
}