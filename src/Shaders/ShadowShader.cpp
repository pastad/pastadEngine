#include  "ShadowShader.h"

#include "Engine.h"
#include "Log.h"
#include "Light.h"

#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"


#include <iostream>
#include <sstream>

ShadowShader::ShadowShader() : Shader()
{
}
ShadowShader::~ShadowShader()
{
}

bool ShadowShader::load(const std::string path)
{  
  if( ! addShader(path + ".vs", VERTEX) )
    return false;

  if( ! addShader(path + ".fs", FRAGMENT) )
    return false;  

  linkAndValidate();

  Engine::getLog()->log("ShadowShader","loaded");
  return true;
}

void ShadowShader::setViewMatrix( glm::mat4 mvmatrix )
{
  bind();
  setUniform("ViewMat",mvmatrix);
  checkUniformError("set ViewMarix ShadowShader"); 
}

void ShadowShader::setProjectionMatrix( glm::mat4 projmatrix )
{
  bind();
  setUniform("ProjectionMat",projmatrix);
  checkUniformError("set ProjectionMatrix ShadowShader");
}
void ShadowShader::setNormalMatrix( glm::mat4 normalmatrix )
{
  bind();
  setUniform("NormalMat",normalmatrix);
  checkUniformError("set NormalMarix ShadowShader");
}

void ShadowShader::use()
{
  Shader::bind();
  checkUniformError("before binding ShadowShader");
}