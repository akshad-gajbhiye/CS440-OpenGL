#version 430 core

layout(location=0) in vec4 coords;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform mat3 normalMat;
uniform uint object;

out vec4 frontAmbDiffExport, frontSpecExport, backAmbDiffExport, backSpecExport;
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
uniform Material canFandB;

vec3 lightDirection, eyeDirection, halfway, normalizedNormal;
vec4 frontEmit, frontGlobAmb, frontAmb, frontDif, frontSpec,
     backEmit, backGlobAmb, backAmb, backDif, backSpec;

void main(void)
{
   texCoordsExport = texCoords;

   normalizedNormal = normalize(normalMat * normal);
   lightDirection = normalize(vec3(light0.coords));
   eyeDirection = -1.0f * normalize(vec3(modelViewMat * coords));
   halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection)/length(lightDirection + eyeDirection);
 
   frontEmit = canFandB.emitCols;
   frontGlobAmb = globAmb * canFandB.ambRefl;
   frontAmb = light0.ambCols * canFandB.ambRefl;
   frontDif = max(dot(normalizedNormal, lightDirection), 0.0f) * (light0.difCols * canFandB.difRefl);    
   frontSpec = pow(max(dot(normalizedNormal, halfway), 0.0f), canFandB.shininess) * (light0.specCols * canFandB.specRefl);
   frontAmbDiffExport =  vec4(vec3(min(frontEmit + frontGlobAmb + frontAmb + frontDif, vec4(1.0))), 1.0);  
   frontSpecExport =  vec4(vec3(min(frontSpec, vec4(1.0))), 1.0);  
   
   normalizedNormal = -1.0f * normalizedNormal;
   backEmit = canFandB.emitCols;
   backGlobAmb = globAmb * canFandB.ambRefl;    
   backAmb = light0.ambCols * canFandB.ambRefl;
   backDif = max(dot(normalizedNormal, lightDirection), 0.0f) * (light0.difCols * canFandB.difRefl);    
   backSpec = pow(max(dot(normalizedNormal, halfway), 0.0f), canFandB.shininess) * (light0.specCols * canFandB.specRefl);
   backAmbDiffExport =  vec4(vec3(min(backEmit + backGlobAmb + backAmb + backDif, vec4(1.0))), 1.0);  
   backSpecExport =  vec4(vec3(min(backSpec, vec4(1.0))), 1.0);  
   
   gl_Position = projMat * modelViewMat * coords;
}