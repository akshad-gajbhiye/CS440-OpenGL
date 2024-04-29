#version 300 es
precision mediump float;

in vec4 frontColsExport, backColsExport;

out vec4 colorsOut;

void main() 
{
  colorsOut = gl_FrontFacing? frontColsExport : backColsExport;
}