export async function setShader(webGLcontext, shaderType, shaderFile) 
{
  let shaderObj;

  // Create a shader object based on type.
  if (shaderType == "vertex") shaderObj = webGLcontext.createShader(webGLcontext.VERTEX_SHADER);
  if (shaderType == "fragment") shaderObj = webGLcontext.createShader(webGLcontext.FRAGMENT_SHADER);

  // Read the source shader file.
  const shaderResponse = await fetch(shaderFile);
  const shaderSource = await shaderResponse.text();
  
  // Compile shader and return shader object.
  const shader = shaderSource.trim();
  webGLcontext.shaderSource(shaderObj, shader);
  webGLcontext.compileShader(shaderObj);
  return shaderObj;
};