#version 430 core

in vec2 texCoordsExport;

uniform sampler2D tex;

out vec4 colorsOut;

void main(void)
{  
   colorsOut = texture(tex, texCoordsExport);
}