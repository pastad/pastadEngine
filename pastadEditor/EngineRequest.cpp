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

AddObjectRequest::AddObjectRequest(std::string path, bool is_static, bool shadow_only, bool is_visible, bool apply_pysics, bool physics_static) : EngineRequest(ERT_ADD_OBJECT), m_path(path), m_is_static(is_static), m_shadow_only(shadow_only), m_is_visible(is_visible), m_apply_physics(apply_pysics),m_physics_static(physics_static)
{
}
AddObjectRequest::~AddObjectRequest() 
{
}
std::string AddObjectRequest::getPath()
{
  return m_path;
}
bool AddObjectRequest::getStatic()
{
  return m_is_static;
}
bool AddObjectRequest::getShadowOnly()
{
  return m_shadow_only;
}
bool AddObjectRequest::getVisible()
{
  return m_is_visible;
}
bool AddObjectRequest::getApplyPhysics()
{
  return m_apply_physics;
}
bool AddObjectRequest::getPhysicsStatic()
{
  return m_physics_static;
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