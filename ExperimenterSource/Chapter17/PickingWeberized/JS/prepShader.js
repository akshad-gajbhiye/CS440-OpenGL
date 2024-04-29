export async function setShader(webGLcontext, shaderType, shaderFile) 
{
  let shaderObj;
  if (shaderType == "vertex") shaderObj = webGLcontext.createShader(webGLcontext.VERTEX_SHADER);
  if (shaderType == "fragment") shaderObj = webGLcontext.createShader(webGLcontext.FRAGMENT_SHADER);

  const shaderResponse = await fetch(shaderFile);
  const shaderSource = await shaderResponse.text();
  const shader = shaderSource.trim();
  webGLcontext.shaderSource(shaderObj, shader);
  webGLcontext.compileShader(shaderObj);
  return shaderObj;
};