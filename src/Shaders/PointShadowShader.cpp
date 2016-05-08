#include  "PointShadowShader.h"

#include "Engine.h"
#include "Log.h"
#include "Light.h"

#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"


#include <iostream>
#include <sstream>

PointShadowShader::PointShadowShader() : Shader()
{
}
PointShadowShader::~PointShadowShader()
{
}

bool PointShadowShader::load(const std::string path)
{  
  if( ! addShader(path + ".vs", VERTEX) )
    return false;

  if( ! addShader(path + ".fs", FRAGMENT) )
    return false;  

  linkAndValidate();

  Engine::getLog()->log("PointShadowShader","loaded");
  return true;
}
void PointShadowShader::setViewMatrix(glm::mat4 view)
{
  bind();  
  setUniform("ViewMat",view);
  checkUniformError("setting ViewMat"); 
}
void PointShadowShader::setLightPosition(glm::vec3 lpos)
{
  bind();
  setUniform("LightPosition",lpos);
  checkUniformError("set LightPosition PointShadowShader");
}

void PointShadowShader::setProjectionMatrix( glm::mat4 projmatrix )
{
  bind();
  setUniform("ProjectionMat",projmatrix);
  checkUniformError("set ProjectionMatrix PointShadowShader");
}
void PointShadowShader::setNormalMatrix( glm::mat4 normalmatrix )
{
  bind();
  setUniform("NormalMat",normalmatrix);
  checkUniformError("set NormalMarix PointShadowShader");
}

void PointShadowShader::use()
{
  Shader::bind();
  checkUniformError("before binding PointShadowShader");
}

void PointShadowShader::setInstanced()
{
  setUniform("Instanced",1);
}
void PointShadowShader::setNotInstanced(glm::mat4 model_transform)
{
  setUniform("Instanced",0);
  setUniform("SingleModelMatrix", model_transform);
}
void PointShadowShader::setBones(std::vector<glm::mat4> * transforms)
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
void PointShadowShader::setAnimation()
{
  bind();
  setUniform("Animation",1);
}
void PointShadowShader::unsetAnimation()
{
  bind();
  setUniform("Animation",0);
}
