
#include <string>

#include "Engine.h"

class Object;
class Light;

enum EngineRequestType
{
  ERT_SHADER_REFRESH,
  ERT_ADD_OBJECT,
  ERT_REMOVE_OBJECT,
  ERT_ADD_LIGHT,
  ERT_REMOVE_LIGHT, 
  ERT_SET_SHADOW_TECHNIQUE_DIRECTIONAL,
  ERT_SET_SHADOW_TECHNIQUE_POINT,
  ERT_SET_SHADOW_TECHNIQUE_SSAO,
  ERT_SET_PP_TECHNIQUE,
  ERT_LOAD_SCENE
};

class EngineRequest
{
public:
  EngineRequest(EngineRequestType type);
  ~EngineRequest();

  EngineRequestType getType();

private:
  EngineRequestType m_type;
};

class RefresShaderRequest : EngineRequest
{
public:
  RefresShaderRequest();
  ~RefresShaderRequest();
};

class LoadSceneRequest : EngineRequest
{
public:
  LoadSceneRequest(std::string path);
  ~LoadSceneRequest();

  std::string getPath();

private:
  std::string m_path;
};


class AddObjectRequest : EngineRequest
{
public:
  AddObjectRequest(std::string path);
  ~AddObjectRequest();

  std::string getPath();

private:
  std::string m_path;
};


class RemoveObjectRequest : EngineRequest
{
public:
  RemoveObjectRequest(Object * obj);
  ~RemoveObjectRequest();

  Object * getObject();
private:
  Object * m_object;  
};

class AddLightRequest : EngineRequest
{
public:
  AddLightRequest(unsigned int type , bool cast);
  ~AddLightRequest();

  unsigned int getLightType();
  bool castsShadow();

private:
  unsigned int m_light_type;
  bool m_cast_shadow;
};

class RemoveLightRequest : EngineRequest
{
public:
  RemoveLightRequest(Light * light);
  ~RemoveLightRequest();

  Light * getLight();
private:
  Light * m_light;
};

class SetShadowTechniqueDirectionalRequest : EngineRequest
{
public:
  SetShadowTechniqueDirectionalRequest(ShadowTechniqueType tech);
  ~SetShadowTechniqueDirectionalRequest();

  ShadowTechniqueType  getShadowTechnique();
private:
  ShadowTechniqueType m_technique;
};
class SetShadowTechniquePointRequest : EngineRequest
{
public:
  SetShadowTechniquePointRequest(ShadowTechniqueType tech);
  ~SetShadowTechniquePointRequest();

  ShadowTechniqueType  getShadowTechnique();
private:
  ShadowTechniqueType m_technique;
}; 

class SetShadowTechniqueAdditionalRequest : EngineRequest
{
public:
  SetShadowTechniqueAdditionalRequest(ShadowTechniqueType tech);
  ~SetShadowTechniqueAdditionalRequest();

  ShadowTechniqueType  getShadowTechnique();
private:
  ShadowTechniqueType m_technique;
};


class SetPPTechniqueRequest : EngineRequest
{
public:
  SetPPTechniqueRequest(PostprocessType tech, bool state);
  ~SetPPTechniqueRequest();

  PostprocessType  getPPTechnique();
  bool getState();
private:
  PostprocessType m_technique;
  bool m_state;
};

