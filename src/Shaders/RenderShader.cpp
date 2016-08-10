#include  "RenderShader.h"

#include "Engine.h"
#include "Log.h"
#include "Light.h"

#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"


#include <iostream>
#include <sstream>

RenderShader::RenderShader() : RenderBaseShader()
{
}

RenderShader::~RenderShader()
{
}


//  load -------------------------------------------------

bool RenderShader::load(const std::string path)
{  
  if( ! addShader(path + ".vs", VERTEX) )
    return false;

  if( ! addShader(path + ".fs", FRAGMENT) )
    return false;

  linkAndValidate();

  Engine::getLog()->log("RenderShader","loaded");
  reset();

  return true;
}


//  getter/setter -------------------------------------------------

void RenderShader::setRenderPass(unsigned int pass)
{
  checkUniformError("pre set Subroutine");
  bind();

  if(pass == 1)
  {
    setRenderPassSubroutine("pass1");
    setUniform("ScreenSpaceRender",0);
  }
  if(pass == 2)
  {
    setRenderPassSubroutine("pass2");
    setUniform("ScreenSpaceRender",1);
  }
  if(pass == 3)
  {
    setRenderPassSubroutine("pass3");
    setUniform("ScreenSpaceRender",1);
  }

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
  ssi << ss.str() <<".ShadowMapIndex";
  setUniform(ssi.str(), (int) light->getShadowIndex());
  ssi.str(""); ssi.clear();

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

  ssi << ss.str() <<".ShadowMapIndex";
  setUniform(ssi.str(), (int)light->getShadowIndex());
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
  ssi << ss.str() <<".ShadowMapIndex";
  setUniform(ssi.str(), (int) pos);
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

void RenderShader::setIdentityMatrices()
{
  glm::mat4 view = glm::mat4(1.0);
  glm::mat4 projection = glm::mat4(1.0);
  setViewMatrix(view);
  setProjectionMatrix(projection);
}

void RenderShader::setMaterial(std::string name, MaterialColorSpecs specs)
{
  // store materials during gbuffer pass and only set index
  int idx;
  std::map<std::string,int>::iterator it = m_materials_mapping.find(name);
  if(it == m_materials_mapping.end())
  {
    if(m_material_number < MAX_NUM_MATERIALS)
    {
      m_materials.insert(std::pair<int, MaterialColorSpecs>(m_material_number,specs) );
      m_materials_mapping.insert(std::pair<std::string,int>(name,m_material_number));
      idx = m_material_number;
      m_material_number++;    
    }
    else
      Engine::getLog()->log("RenderShader","to many materials allready set");
  } 
  else
    idx = it->second;

  //only set the id for now
  setMaterialIndex(idx);
  if(idx >= m_material_number)
    assert(0);

  //==std::cout<< "Mat count:" << m_materials_mapping.size()<<std::endl;
}

std::vector<int> RenderShader::getEmmissiveMaterialIndices()
{
  std::vector<int> ret;

  for(std::map<std::string, int>::iterator it = m_materials_mapping.begin(); it != m_materials_mapping.end();it++)
  {
    int idx = it->second;
    std::map<int, MaterialColorSpecs>::iterator it2 = m_materials.find(idx);
    if(it2 != m_materials.end())
    {      
     if( it2->second.m_emmissive != 0.0f )
        ret.push_back(idx);   
    }
  }

  return ret;
}

void RenderShader::setMaterialIndex(int idx)
{
   setUniform("MaterialIndex",idx);
}

void RenderShader::setAllMaterialsForRenderPass()
{
  //std::cout<< "Mat count set:" << m_materials_mapping.size()<<std::endl;
  // set everything for lighting pass | we have the idx in the gbuffer
  for(std::map<std::string, int>::iterator it = m_materials_mapping.begin(); it != m_materials_mapping.end();it++)
  {
    int idx = it->second;
    std::map<int, MaterialColorSpecs>::iterator it2 = m_materials.find(idx);
    if(it2 != m_materials.end())
    {      
      std::stringstream ss,ssi;
      ss<< "Materials["<<idx <<"].";

      ssi<< ss.str() <<"DiffuseColor";
      setUniform(ssi.str(),it2->second.m_diffuse_color);
      ssi.str(""); ssi.clear();

      ssi<< ss.str() <<"AmbientColor";
      setUniform(ssi.str(),it2->second.m_ambient_color);
      ssi.str(""); ssi.clear();

      ssi<< ss.str() <<"SpecularColor";
      setUniform(ssi.str(),it2->second.m_specular_color);
      ssi.str(""); ssi.clear();

      ssi<< ss.str() <<"Shininess";
      setUniform(ssi.str(),it2->second.m_shininess);
      ssi.str(""); ssi.clear();

      ssi<< ss.str() <<"Opacity";
      setUniform(ssi.str(),it2->second.m_opacity);
      ssi.str(""); ssi.clear();

      checkUniformError("set material specs");
 
    }
    else
    {
      Engine::getLog()->log("RenderShader", "error while setting materials");
    }
  }
}

int RenderShader::setShadowMap(glm::mat4 shadow_mat)
{
  int c = m_shadow_map_count;
  std::stringstream ss;
  ss << "ShadowMatrices[" << c << "]";
  setUniform(ss.str(),shadow_mat );
  checkUniformError("set shadow map");
  m_shadow_map_count++;
 // Engine::getLog()->log("RenderShader",ss.str(), " shadow maps set");
  return c;
}

void RenderShader::resetShadowMapping()
{
  m_shadow_map_count =m_cube_shadow_map_count = 0;
  checkUniformError("resset shadow map count");
}

int RenderShader::setPointShadow()
{
  int ret = m_cube_shadow_map_count;
  m_cube_shadow_map_count++;

  return ret;
}

void RenderShader::setShadows(ShadowTechniqueType tech)
{
  if(tech == ST_NONE)
  {
    setUniform("EnableShadows",0 );
    resetShadowMapping();
    Engine::getLog()->log("RenderShader", "Disable Shadows");     
  }
  if(tech == ST_STANDARD)
  {
    setUniform("EnableShadows",1 );
    Engine::getLog()->log("RenderShader", "Enable Standard Shadows");     
  }
  if(tech == ST_STANDARD_PCF)
  {
    setUniform("EnableShadows",2 );
    Engine::getLog()->log("RenderShader", "Enable Standard PCF Shadows");     
  }
  if(tech == ST_STANDARD_RS)
  {
    setUniform("EnableShadows",3 );
    Engine::getLog()->log("RenderShader", "Enable Standard RandomSampling Shadows");     
  }
  checkUniformError("set standard shadows");
}

void RenderShader::setFog(glm::vec3 color, float factor,float offset)
{
  setUniform("FogColor",color );
  setUniform("FogFactor",factor );
  setUniform("FogOffset",offset );
}

//  use/reset -------------------------------------------------

void RenderShader::use()
{
  checkUniformError("at binding"); 
  Shader::bind();
}
void RenderShader::reset()
{
  // reset at begin of render
 // m_materials.clear();
//  m_materials_mapping.clear();
  //m_material_number = 0;
}