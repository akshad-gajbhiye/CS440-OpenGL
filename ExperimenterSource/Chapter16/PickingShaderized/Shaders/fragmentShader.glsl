#version 430 core

in vec4 colorsExport;

uniform vec4 colorCode, highlightColor;

uniform uint isSelecting, isHighlight;

out vec4 colorsOut;

void main(void)
{
   if (isSelecting == 1) colorsOut = colorCode; // Draw color coded.
   else if (isHighlight == 1) colorsOut = highlightColor; // Draw highlighted.
   else colorsOut = colorsExport; // Draw original colors.
}