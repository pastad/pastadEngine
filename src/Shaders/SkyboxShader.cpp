#include "SkyboxShader.h"

#include "Engine.h"
#include "Log.h"

#include "glm/ext.hpp"

SkyboxShader::SkyboxShader()
{  
}

SkyboxShader::~SkyboxShader()
{  
}


//  load -------------------------------------------------

bool SkyboxShader::load(const std::string path)
{
  if( ! addShader(path + ".vs", VERTEX) )
    return false;

  if( ! addShader(path + ".fs", FRAGMENT) )
    return false;

  linkAndValidate();

  Engine::getLog()->log("SkyboxShader","loaded"); 

  return true;
}


//  getter/setter -------------------------------------------------

void SkyboxShader::setView(glm::mat4 view)
{
  setUniform("ViewMat",view);
  checkUniformError("set ViewMarix"); 
}

void SkyboxShader::setProjection(glm::mat4 projection)
{
  setUniform("ProjectionMat",projection);

  checkUniformError("set projectionMarix"); 
}

//  use -------------------------------------------------

void SkyboxShader::use()
{
  checkUniformError("sbs at binding"); 
  Shader::bind();
}