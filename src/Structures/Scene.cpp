#include "Scene.h"

#include "RessourceManager.h"
#include "RenderShader.h"
#include "ShadowShader.h"
#include "PointShadowShader.h"
#include "Model.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Engine.h"
#include "Log.h"
#include "Skybox.h"
#include "SkyboxShader.h"

Scene::Scene()
{
  m_camera = new Camera(0,2,0);
  m_skybox = nullptr;
  
}
Scene::~Scene()
{  
  for(std::vector<Object *>::iterator it = m_objects.begin(); it != m_objects.end();it++)
    delete (*it);
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
    delete (*it);
  delete m_skybox;
  delete m_camera;
}

void Scene::update(float delta)
{
  m_camera->update(delta);
}

void Scene::render(RenderShader * render_shader, SkyboxShader * skybox_shader)
{  
  render_shader->setLights(&m_lights);
  render_shader->setCameraPosition(m_camera->getPosition());

	for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
    it->second->render(render_shader);

  renderSkybox(skybox_shader);

}
void Scene::renderShadow(ShadowShader * shadow_shader, PointShadowShader* point_shadow_shader)
{  
  glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
  {
    if( (*it)->getShadowRefresh() )
    {
      Engine::getLog()->log("Scene", "re-render light");
      if( (*it)->getType() == LIGHT_SPOT )
      {
        shadow_shader->use();
        (*it)->bindForShadowRenderSpot(shadow_shader);

        for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
          it->second->renderWithoutMaterial();

        (*it)->unbindFromShadowRender();
      }
      if( (*it)->getType() == LIGHT_POINT )
      {        
        point_shadow_shader->use();
        for( int iteration =0; iteration <6; iteration++)
        {
          (*it)->bindForShadowRenderPoint(point_shadow_shader,iteration);
          for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
            it->second->renderWithoutMaterial();
          (*it)->unbindFromShadowRender(); 
        }            
      }
    }
  }
}

void Scene::renderSkybox(SkyboxShader * skybox_shader)
{
  skybox_shader->use();

  skybox_shader->setView(m_camera->getViewWithoutTranslation());
  skybox_shader->setProjection(m_camera->getProjection());

  if(m_skybox != nullptr)
    m_skybox->render();
}

bool Scene::setSkybox(const std::string path)
{
  if(m_skybox == nullptr)
    m_skybox = new Skybox();

  if(!m_skybox->load(path))
  {
    delete m_skybox;
    m_skybox = nullptr;
    return false;  
  }

  return true;
}


void Scene::setupLightsForShadingPass(RenderShader * render_shader)
{
  render_shader->resetShadowMapping();
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
  {
    if( (*it)->getType() == LIGHT_SPOT )
      (*it)->bindForRender(render_shader);
    if( (*it)->getType() == LIGHT_POINT )
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
