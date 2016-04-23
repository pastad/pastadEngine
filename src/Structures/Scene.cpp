#include "Scene.h"

#include "RessourceManager.h"
#include "RenderShader.h"
#include "ShadowShader.h"
#include "Model.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"

Scene::Scene()
{
  m_camera = new Camera(0,0,0);
  
}
Scene::~Scene()
{  
  for(std::vector<Object *>::iterator it = m_objects.begin(); it != m_objects.end();it++)
    delete (*it);
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
    delete (*it);
}

void Scene::update(float delta)
{
  m_camera->update(delta);
}

void Scene::render(RenderShader * render_shader)
{  
  render_shader->setLights(&m_lights);
  render_shader->setCameraPosition(m_camera->getPosition());

	for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
    it->second->render(render_shader);
}
void Scene::renderShadow(ShadowShader * shadow_shader)
{  
  shadow_shader->use();
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
  {
    if( (*it)->getType() == LIGHT_SPOT )
    {
      (*it)->bindForShadowRender(shadow_shader);

      for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
        it->second->renderWithoutMaterial();

      (*it)->unbindFromShadowRender();
    }
  }
}
void Scene::setupLightsForShadingPass(RenderShader * render_shader)
{
  render_shader->resetShadowMapping();
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
  {
    (*it)->bindForRender(render_shader);
  }
}

Object * Scene::addObject(std::string path)
{
  Model * m = RessourceManager::loadModel(path);
  Object * obj = m->getInstance();
  m_objects.insert(m_objects.end(),obj);
  m_models[path] = m;

  return obj;
}
Light * Scene::addLight()
{
  Light * light = new Light();
  m_lights.insert(m_lights.end(), light);  
  return light; 
}

Camera * Scene::getCamera()
{
  return m_camera;
}
