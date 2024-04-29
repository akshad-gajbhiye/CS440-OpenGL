#version 430 core

#define FIELD 0
#define SKY 1
#define LINE 2

layout(location=0) in vec4 coords;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform mat3 normalMat;
uniform uint object;

out vec4 fAndBColsExport;
out vec2 texCoordsExport;

struct Light
{
   vec4 ambCols;
   vec4 difCols;
   vec4 specCols;
   vec4 coords;
};
uniform Light light0;

uniform vec4 globAmb;
  
struct Material
{
   vec4 ambRefl;
   vec4 difRefl;
   vec4 specRefl;
   vec4 emitCols;
   float shininess;
};
uniform Material matFandB;

vec3 lightDirection, eyeDirection, halfway, normalizedNormal;
vec4 fAndBEmit, fAndBGlobAmb, fAndBAmb, fAndBDif, fAndBSpec;

void main(void)
{   
   if (object == FIELD)
   {
      texCoordsExport = texCoords;
	  
      normalizedNormal = normalize(normalMat * normal);
      lightDirection = normalize(vec3(light0.coords));
      eyeDirection = vec3(0.0, 0.0, 1.0);
      halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection)/length(lightDirection + eyeDirection);
   
      fAndBEmit = matFandB.emitCols;
      fAndBGlobAmb = globAmb * matFandB.ambRefl;
      fAndBAmb = light0.ambCols * matFandB.ambRefl;
      fAndBDif = max(dot(normalizedNormal, lightDirection), 0.0f) * (light0.difCols * matFandB.difRefl);    
      fAndBSpec = pow(max(dot(normalizedNormal, halfway), 0.0f), matFandB.shininess) * (light0.specCols * matFandB.specRefl);
      fAndBColsExport =  vec4(vec3(min(fAndBEmit + fAndBGlobAmb + fAndBAmb + fAndBDif + fAndBSpec, vec4(1.0))), 1.0);  
      }
   if (object == SKY) texCoordsExport = texCoords;

   gl_Position = projMat * modelViewMat * coords;
}
