#include  "RenderShader.h"

#include "Engine.h"
#include "Log.h"
#include "Light.h"

#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"


#include <iostream>
#include <sstream>

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
  setUniform("Material.DiffuseColor",specs.m_diffuse_color);
  setUniform("Material.AmbientColor",specs.m_ambient_color);
  setUniform("Material.SpecularColor",specs.m_specular_color);
  setUniform("Material.Shininess",specs.m_shininess);
  setUniform("Material.Opacity",specs.m_opacity);
  checkUniformError("set material specs");
}
void RenderShader::setLights(std::vector<Light*> * lights)
{
  int num_directional_lights = 0;
  int num_spot_lights = 0;
  int num_point_lights = 0;

  for(std::vector<Light *>::iterator it = lights->begin(); it != lights->end();it++)
  {
    unsigned int type = (*it)->getType();

    if(type == LIGHT_DIRECTIONAL)
    {
      setDirectionalLight( (*it), num_directional_lights );
      num_directional_lights++;
    }
    if(type == LIGHT_SPOT)
    {
      setSpotLight( (*it), num_spot_lights );
      num_spot_lights++;
    }
    if(type == LIGHT_POINT)
    {
      setPointLight( (*it), num_point_lights );
      num_point_lights++;
    }
  }
  setUniform("NumDirectionalLights", num_directional_lights);
  setUniform("NumSpotLights", num_spot_lights);
  setUniform("NumPointLights", num_point_lights);

}
void RenderShader::setDirectionalLight(Light* light, unsigned int pos)
{
  std::stringstream ss, ssi;
  ss << "DirectionalLights["<<pos<<"]";

  // base light
  ssi << ss.str() <<".Direction";
  setUniform(ssi.str(), light->getDirection());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Base.AmbientColor";
  setUniform(ssi.str(), light->getAmbientColor());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Base.DiffuseColor";
  setUniform(ssi.str(), light->getDiffuseColor());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Base.SpecularColor";
  setUniform(ssi.str(), light->getSpecularColor());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Base.Intensity";
  setUniform(ssi.str(), light->getIntensity());
  ssi.str(""); ssi.clear();
}
void RenderShader::setSpotLight(Light* light, unsigned int pos)
{
  std::stringstream ss, ssi;
  ss << "SpotLights["<<pos<<"]";

  ssi << ss.str() <<".Direction";
  setUniform(ssi.str(), light->getDirection());
  ssi.str(""); ssi.clear();

  ssi << ss.str() <<".CutoffAngle";
  setUniform(ssi.str(), light->getCutoffAngle());
  ssi.str(""); ssi.clear();


  // point light 
  ssi << ss.str() <<".Pointlight.Position";
  setUniform(ssi.str(), light->getPosition());
  ssi.str(""); ssi.clear();

  // attenuation
  ssi << ss.str() <<".Pointlight.Attenuation.Constant";
  setUniform(ssi.str(), light->getAttenuationConstant());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Pointlight.Attenuation.Linear";
  setUniform(ssi.str(), light->getAttenuationLinear());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Pointlight.Attenuation.Quadratic";
  setUniform(ssi.str(), light->getAttenuationQuadratic());
  ssi.str(""); ssi.clear();

  // base light
  ssi << ss.str() <<".Pointlight.Base.AmbientColor";
  setUniform(ssi.str(), light->getAmbientColor());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Pointlight.Base.DiffuseColor";
  setUniform(ssi.str(), light->getDiffuseColor());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Pointlight.Base.SpecularColor";
  setUniform(ssi.str(), light->getSpecularColor());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Pointlight.Base.Intensity";
  setUniform(ssi.str(), light->getIntensity());
  ssi.str(""); ssi.clear();
}
void RenderShader::setPointLight(Light* light, unsigned int pos)
{
  std::stringstream ss, ssi;
  ss << "PointLights["<<pos<<"]";

  ssi << ss.str() <<".Position";
  setUniform(ssi.str(), light->getPosition());
  ssi.str(""); ssi.clear();

  // attenuation
  ssi << ss.str() <<".Attenuation.Constant";
  setUniform(ssi.str(), light->getAttenuationConstant());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Attenuation.Linear";
  setUniform(ssi.str(), light->getAttenuationLinear());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Attenuation.Quadratic";
  setUniform(ssi.str(), light->getAttenuationQuadratic());
  ssi.str(""); ssi.clear();

  // base light
  ssi << ss.str() <<".Base.AmbientColor";
  setUniform(ssi.str(), light->getAmbientColor());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Base.DiffuseColor";
  setUniform(ssi.str(), light->getDiffuseColor());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Base.SpecularColor";
  setUniform(ssi.str(), light->getSpecularColor());
  ssi.str(""); ssi.clear();
  ssi << ss.str() <<".Base.Intensity";
  setUniform(ssi.str(), light->getIntensity());
  ssi.str(""); ssi.clear();
}
void RenderShader::setCameraPosition(glm::vec3 pos)
{
  setUniform("CameraPosition", pos);
}




void RenderShader::use()
{
  Shader::bind();
}