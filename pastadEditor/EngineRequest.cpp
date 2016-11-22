#include "EngineRequest.h"

#include "Object.h"

#include <string>


EngineRequest::EngineRequest(EngineRequestType type): m_type(type)
{
}
EngineRequest::~EngineRequest()
{
}
EngineRequestType EngineRequest::getType()
{
  return m_type;
}

RefresShaderRequest::RefresShaderRequest(): EngineRequest(ERT_SHADER_REFRESH)
{
}
RefresShaderRequest::~RefresShaderRequest()
{
}

AddObjectRequest::AddObjectRequest(std::string path) : EngineRequest(ERT_ADD_OBJECT), m_path(path)
{
}
AddObjectRequest::~AddObjectRequest() 
{
}
std::string AddObjectRequest::getPath()
{
  return m_path;
}

RemoveObjectRequest::RemoveObjectRequest(Object * object) : EngineRequest(ERT_REMOVE_OBJECT)
{
  m_object = object;
}
RemoveObjectRequest::~RemoveObjectRequest()
{
}
Object * RemoveObjectRequest::getObject()
{
  return m_object;
}


AddLightRequest::AddLightRequest(unsigned int type, bool cast) : EngineRequest(ERT_ADD_LIGHT), m_light_type(type), m_cast_shadow(cast)
{
}
AddLightRequest::~AddLightRequest()
{
}
unsigned int AddLightRequest::getLightType()
{
  return m_light_type;
}
bool AddLightRequest::castsShadow()
{
  return m_cast_shadow;
}

RemoveLightRequest::RemoveLightRequest(Light * light) : EngineRequest(ERT_REMOVE_LIGHT)
{
  m_light = light;
}
RemoveLightRequest::~RemoveLightRequest()
{
}
Light * RemoveLightRequest::getLight()
{
  return m_light;
}

SetShadowTechniqueDirectionalRequest::SetShadowTechniqueDirectionalRequest(ShadowTechniqueType tech) : EngineRequest(ERT_SET_SHADOW_TECHNIQUE_DIRECTIONAL)
{
  m_technique = tech;
}
SetShadowTechniqueDirectionalRequest::~SetShadowTechniqueDirectionalRequest()
{
}
ShadowTechniqueType SetShadowTechniqueDirectionalRequest::getShadowTechnique()
{
  return m_technique;
}
SetShadowTechniquePointRequest::SetShadowTechniquePointRequest(ShadowTechniqueType tech) : EngineRequest(ERT_SET_SHADOW_TECHNIQUE_POINT)
{
  m_technique = tech;
}
SetShadowTechniquePointRequest::~SetShadowTechniquePointRequest()
{
}
ShadowTechniqueType SetShadowTechniquePointRequest::getShadowTechnique()
{
  return m_technique;
}

SetShadowTechniqueAdditionalRequest::SetShadowTechniqueAdditionalRequest(ShadowTechniqueType tech) : EngineRequest(ERT_SET_SHADOW_TECHNIQUE_SSAO)
{
  m_technique = tech;
}
SetShadowTechniqueAdditionalRequest::~SetShadowTechniqueAdditionalRequest()
{
}
ShadowTechniqueType  SetShadowTechniqueAdditionalRequest::getShadowTechnique()
{
  return m_technique;
}


SetPPTechniqueRequest::SetPPTechniqueRequest(PostprocessType tech, bool state) : EngineRequest(ERT_SET_PP_TECHNIQUE), m_state(state)
{
  m_technique = tech;
}
SetPPTechniqueRequest::~SetPPTechniqueRequest()
{
}
PostprocessType  SetPPTechniqueRequest::getPPTechnique()
{
  return m_technique;
}
bool SetPPTechniqueRequest::getState()
{
  return m_state;
}


LoadSceneRequest::LoadSceneRequest(std::string path) : EngineRequest(ERT_LOAD_SCENE), m_path(path)
{
}
LoadSceneRequest::~LoadSceneRequest()
{
}
std::string LoadSceneRequest::getPath()
{
  return m_path;
}