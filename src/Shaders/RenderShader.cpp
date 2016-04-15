#include  "RenderShader.h"

#include "Engine.h"
#include "Log.h"

#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"


#include<iostream>

RenderShader::RenderShader() : Shader()
{
}
RenderShader::~RenderShader()
{
}

bool RenderShader::load(const std::string path)
{  
  if( ! addShader(path + ".vs", VERTEX) )
    return false;

  if( ! addShader(path + ".fs", FRAGMENT) )
    return false;

  linkAndValidate();

  Engine::getLog()->log("RenderShader","loaded");

  return true;
}

void RenderShader::setViewMatrix( glm::mat4 mvmatrix )
{
  bind();
  setUniform("ViewMat",mvmatrix);
  checkUniformError("set ViewMarix"); 
}
void RenderShader::setProjectionMatrix( glm::mat4 projmatrix )
{
  bind();
  setUniform("ProjectionMat",projmatrix);
  checkUniformError("set ProjectionMarix");
}
void RenderShader::setNormalMatrix( glm::mat4 normalmatrix )
{
  bind();
  setUniform("NormalMat",normalmatrix);
  checkUniformError("set NormalMarix");
}

void RenderShader::setRenderPass(unsigned int pass)
{
  bind();
  if(pass == 1)
    setRenderPassSubroutine("pass1");
  else
    setRenderPassSubroutine("pass2");

  checkUniformError("set Subroutine");
  //printUniforms();
}
void RenderShader::setColorOnly(bool color_only)
{
  if(color_only)
    setUniform("ColorOnly",1);
  else
    setUniform("ColorOnly",0);
}
void RenderShader::setMaterial(MaterialColorSpecs specs)
{
  setUniform("Material.m_diffuse_color",specs.m_diffuse_color);
  setUniform("Material.m_ambientcolor",specs.m_ambient_color);
  setUniform("Material.m_specular_color",specs.m_specular_color);
  setUniform("Material.m_shininess",specs.m_shininess);
  setUniform("Material.m_opacity",specs.m_opacity);
  checkUniformError("set material specs");
}


void RenderShader::use()
{
  Shader::bind();
}