#version 430 core

layout(location=0) in vec4 coords;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform mat3 normalMat;
uniform vec4 clipPlane;

out vec3 normalExport;
out vec2 texCoordsExport;
out vec3 eyeDirectionExport;

vec3 lightDirection, eyeDirection, halfway;
vec4 frontEmit, frontGlobAmb, frontAmb, frontDif, frontSpec,
     backEmit, backGlobAmb, backAmb, backDif, backSpec;

float gl_ClipDistance[1];

void main(void)
{
   normalExport = normalMat * normal;
   texCoordsExport = texCoords;
   eyeDirectionExport = -1.0f * normalize(vec3(modelViewMat * coords));

   gl_Position = projMat * modelViewMat * coords;
   gl_ClipDistance[0] = dot(clipPlane, coords);
}