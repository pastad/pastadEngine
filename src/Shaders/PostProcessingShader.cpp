#include  "PostProcessingShader.h"

#include "Engine.h"
#include "Log.h"

#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"


#include <iostream>
#include <sstream>

PostProcessingShader::PostProcessingShader() : Shader()
{
}
PostProcessingShader::~PostProcessingShader()
{
}

bool PostProcessingShader::load(const std::string path)
{  
  if( ! addShader(path + ".vs", VERTEX) )
    return false;

  if( ! addShader(path + ".fs", FRAGMENT) )
    return false;

  linkAndValidate();

  Engine::getLog()->log("PostProcessingShader","loaded");

  return true;
}

void PostProcessingShader::setRenderPass(unsigned int pass)
{
  bind();
  if(pass == 1)
  {
    setRenderPassSubroutine("pass1");
  }
  if(pass == 2)
  {
    setRenderPassSubroutine("pass2");
  }
  if(pass == 3)
  {
    setRenderPassSubroutine("pass3");
  }

  checkUniformError("set Subroutine");
  //printUniforms();
}

void PostProcessingShader::use()
{
  Shader::bind();
}

void PostProcessingShader::setIdentityMatrices()
{
  glm::mat4 view = glm::mat4(1.0);
  glm::mat4 projection = glm::mat4(1.0);
  setUniform("ViewMat",view);
  setUniform("ProjectionMat",projection);
  checkUniformError("pp view projection set");
}


void PostProcessingShader::setTextureScaling(glm::vec2 scale)
{
  setUniform("TextureScale",scale);    
  checkUniformError("texture scale set");
}