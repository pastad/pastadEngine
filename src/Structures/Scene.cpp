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
#include "SceneTreeElement.h"

#include <iostream>

Scene::Scene()
{
  m_camera = new Camera(0,2,0);
  m_skybox = nullptr;
  m_tree_root= new SceneTreeElement(500, glm::vec3(0,0,0));
  
}
Scene::~Scene()
{  
  for(std::vector<Object *>::iterator it = m_objects.begin(); it != m_objects.end();it++)
    delete (*it);
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
    delete (*it);
  if(m_skybox != nullptr)
    delete m_skybox;
  if(m_camera != nullptr)
    delete m_camera;
  if(m_tree_root != nullptr)
    delete m_tree_root;
}

void Scene::update(float delta)
{
  m_camera->update(delta);
  update( m_tree_root,delta);
}
void Scene::update(SceneTreeElement * element,  float delta)
{
  std::vector< SceneTreeElement *> children = element->getChildren();
  std::vector< Object *> objects = element->getObjects();
  for( int x=0;  x< objects.size(); x++)
  {
    Object * obj = objects.at(x);
    obj->advanceAnimation(delta);    
  }

  for( int x=0;  x< children.size(); x++)
  {
    update(children.at(x), delta );
  }
}

void Scene::render(RenderShader * render_shader, SkyboxShader * skybox_shader)
{  
  render_shader->setLights(&m_lights);
  render_shader->setCameraPosition(m_camera->getPosition());

 // float angle_mid = glm::orientedAngle(glm::vec2(1,1),  glm::vec2( v.x ,v.z)   );

  // render the accumulated models
	for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
  {
    std::map<std::string, std::vector<Object *>>::iterator objs =m_render_objects.find(it->first);
    if(objs !=  m_render_objects.end() )
    {
      if(objs->second.size() >0)
        it->second->render(render_shader ,objs->second);
    }
  }

  renderSkybox(skybox_shader);

}
void Scene::renderShadow(ShadowShader * shadow_shader, PointShadowShader* point_shadow_shader)
{  
  glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
  {
    if( (*it)->getShadowRefresh() )
    {
      //Engine::getLog()->log("Scene", "re-render light");
      if( ( (*it)->getType() == LIGHT_SPOT )  )
      {
        shadow_shader->use();
        (*it)->bindForShadowRenderSpot(shadow_shader);

        for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
          it->second->renderWithoutMaterial(shadow_shader,nullptr);

        (*it)->unbindFromShadowRender();
      }
       if( ( (*it)->getType() == LIGHT_DIRECTIONAL )  )
      {
        shadow_shader->use();
        (*it)->bindForShadowRenderDirectional(shadow_shader);

        for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
          it->second->renderWithoutMaterial(shadow_shader,nullptr);

        (*it)->unbindFromShadowRender();
      }
      if( (*it)->getType() == LIGHT_POINT )
      {        
        point_shadow_shader->use();
        for( int iteration =0; iteration <6; iteration++)
        {
          (*it)->bindForShadowRenderPoint(point_shadow_shader,iteration);
          for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
            it->second->renderWithoutMaterial(nullptr,point_shadow_shader);
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
    if( (*it)->getType() == LIGHT_DIRECTIONAL )
      (*it)->bindForRender(render_shader);
  }
}

Object * Scene::addObject(std::string path, glm::vec3 position)
{
  return addObject(path,position,false);
}
Object * Scene::addObjectInstanced(std::string path, glm::vec3 position)
{
  return addObject(path,position,true);
}
Object * Scene::addObject(std::string path, glm::vec3 position, bool instanced)
{
  Model * m = RessourceManager::loadModel(path, instanced);
  Object * obj = m->getInstance();
  obj->setPosition(position);
  m_objects.insert(m_objects.end(),obj);
  m_models[path] = m;

  m_tree_root->insert(obj);

  refreshRenderObjects();

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

void Scene::cameraMoved()
{
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
  {
    if( (*it)->getType() == LIGHT_DIRECTIONAL )
      (*it)->refresh();
  }
  refreshRenderObjects();
}
void Scene::cameraRotated()
{
  refreshRenderObjects();
}

void Scene::refreshRenderObjects()
{
  m_render_objects.clear();
  glm::vec3 v = m_camera->getDirection();
  v = glm::normalize(v);

  for(std::vector<Object *>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
  {
    glm::vec3 dir_to_object  = (*it)->getPosition() - m_camera->getPosition();
    float angle_mid = std::acos(glm::dot( glm::vec2(dir_to_object.x,dir_to_object.z), glm::vec2( v.x ,v.z) ) / ( glm::length(glm::vec2(dir_to_object.x,dir_to_object.z)) * glm::length(glm::vec2( v.x ,v.z) )  )); 
    angle_mid =  glm::degrees(angle_mid);    

    /* tmp disable due to flickers 
    if( ( ((*it)->getPriorityRender() ) || (angle_mid < m_camera->getFOV()) )  && ((*it)->isVisible())  ) //
    {
      //std::cout << angle_mid << std::endl;
      //std::cout << (*it)->getIdentifier()<<std::endl;
    */
      std::map< std::string , std::vector<Object *> >::iterator entry = m_render_objects.find( (*it)->getIdentifier() );
      if(entry != m_render_objects.end())
      {
        entry->second.insert(entry->second.end(), (*it));
      }
      else
      {
        std::vector<Object *> ins_vec;
        ins_vec.insert(ins_vec.end(), (*it) );
        m_render_objects.insert(std::pair< std::string , std::vector<Object *> >((*it)->getIdentifier(), ins_vec) );
      }
    //} tmp disable due to flickers 
  }
}

SceneTreeElement * Scene::getSceneRoot()
{
  return m_tree_root;
}