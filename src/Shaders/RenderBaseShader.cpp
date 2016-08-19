#include "RenderBaseShader.h"

#include "Engine.h"
#include "Log.h"

#include <sstream>

RenderBaseShader::RenderBaseShader():Shader()
{
}

RenderBaseShader::~RenderBaseShader()
{  
}


//  load -------------------------------------------------

bool RenderBaseShader::load(const std::string path)
{  
  if( ! addShader(path + ".vs", VERTEX) )
    return false;

  if( ! addShader(path + ".fs", FRAGMENT) )
    return false;

  linkAndValidate();

  Engine::getLog()->log("RenderBaseShader","loaded");
  return true;
}

//  getter/setter -------------------------------------------------

void RenderBaseShader::setAnimation()
{
  bind();
  setUniform("Animation",1);
}

void RenderBaseShader::unsetAnimation()
{
  bind();
  setUniform("Animation",0);
}

void RenderBaseShader::setInstanced()
{
  bind();
  setUniform("Instanced",1);
}

void RenderBaseShader::setNotInstanced(glm::mat4 model_transform)
{
  bind();
  setUniform("Instanced",0);
  setUniform("SingleModelMatrix", model_transform);
}

void RenderBaseShader::setBones(std::vector<glm::mat4> * transforms)
{
  int counter =0;
  for(std::vector<glm::mat4>::iterator it = transforms->begin(); it != transforms->end(); it++)
  {
    std::stringstream ss;
    ss << "Bones[" << counter << "]";
    counter++;
    setUniform(ss.str(), (*it) );
  }
}

void RenderBaseShader::setViewMatrix( glm::mat4 mvmatrix )
{
  bind();
  setUniform("ViewMat",mvmatrix);
  checkUniformError("set ViewMarix ShadowShader"); 
}

void RenderBaseShader::setProjectionMatrix( glm::mat4 projmatrix )
{
  bind();
  setUniform("ProjectionMat",projmatrix);
  checkUniformError("set ProjectionMatrix ShadowShader");
}

void RenderBaseShader::setNormalMatrix( glm::mat4 normalmatrix )
{
  bind();
  setUniform("NormalMat",normalmatrix);
  checkUniformError("set NormalMarix ShadowShader");
}

void RenderBaseShader::setIndentifcation(int idx)
{
  bind();
  setUniform("ObjectId", idx );
}

//  use -------------------------------------------------

void RenderBaseShader::use()
{
  Shader::bind();
  checkUniformError("before binding ShadowShader");
}
