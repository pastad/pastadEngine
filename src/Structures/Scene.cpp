#include "Scene.h"

#include "RessourceManager.h"
#include "RenderShader.h"
#include "RenderBaseShader.h"
#include "Model.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Engine.h"
#include "Log.h"
#include "Skybox.h"
#include "SkyboxShader.h"
#include "Terrain.h"
#include "SceneTreeElement.h"

#include <iostream>
#include <future>

#include "tinyxml2.h"

Scene::Scene()
{
  m_object_counter = 1;
  m_camera = new Camera(0,2,0);
  m_skybox =  nullptr;
  m_terrain = nullptr;
  m_tree_root= new SceneTreeElement(500, glm::vec3(0,0,0));
  
}
Scene::~Scene()
{  
  for(std::vector<Object *>::iterator it = m_objects.begin(); it != m_objects.end();it++)
  {
    if( (*it) != nullptr)
      delete (*it);
  }
  m_objects.clear();
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
  {
    if( (*it) != nullptr)
      delete (*it);
  }
  m_lights.clear();
  if(m_skybox != nullptr)
    delete m_skybox;
  if(m_camera != nullptr)
    delete m_camera;
  if(m_tree_root != nullptr)
    delete m_tree_root;
  if(m_terrain != nullptr)
    delete m_terrain;
  Engine::getLog()->log("Scene", "deleted");
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

  std::vector<std::future<void>> updated;
  for( int x=0;  x< objects.size(); x++)
  {    
    Object * obj =objects.at(x);
    obj->advanceAnimation(delta); 
  }
  for( int x=0;  x< children.size(); x++)
  {
    update(children.at(x), delta );
  }

}

void Scene::render(RenderShader * render_shader, SkyboxShader * skybox_shader, RenderBaseShader * terrain_shader)
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
        it->second->render((RenderBaseShader *)render_shader ,objs->second, true);
    }
  }
  int c = 0;
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
  {
    (*it)->editRender(render_shader,c);
    if((*it)->getType()  == LIGHT_DIRECTIONAL)
      c++;
  }

  if(m_terrain != nullptr)
    m_terrain->render(terrain_shader);

  renderSkybox(skybox_shader);

}




void Scene::renderShadow(RenderBaseShader * shadow_shader, RenderBaseShader* point_shadow_shader)
{  
  glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
  {
    if( (*it)->getShadowRefresh() && (*it)->isShadowEnabled())
    {
      Engine::getLog()->log("Scene", "re-render light");
      if( (*it)->getType() == LIGHT_SPOT )  
      {
        shadow_shader->use();
        (*it)->bindForShadowRenderDirectional(shadow_shader);
        

        for(std::map<std::string, Model *>::iterator it2 = m_models.begin(); it2 != m_models.end();it2++)
        {
          std::vector<Object *> objs = it2->second->getObjects();
          for(std::vector<Object *>::iterator it3 = objs.begin(); it3 != objs.end();)
          {         
            float angle = (*it3)->getMinAngleToLight( (*it) );
            glm::vec3 p = (*it3)->getMinBBDistantPoint( (*it)->getPosition() ) ;
            //std::cout <<angle<<std::endl;
            if( (! (*it)->isInRange(p) ) || (angle > (*it)->getCutoffAngle() ) )
              objs.erase(it3);
            else
              it3++;
          }
          //std::cout << objs.size()<<std::endl;
          it2->second->render((RenderBaseShader *)shadow_shader,objs, false);

          //it2->second->render(shadow_shader, false);
        }
        

        if(m_terrain != nullptr)
          m_terrain->renderWithoutMaterial(shadow_shader);

        (*it)->unbindFromShadowRender();
      }
      if( (*it)->getType() == LIGHT_DIRECTIONAL )  
      {
        shadow_shader->use();
        (*it)->bindForShadowRenderDirectional(shadow_shader);

        for(std::map<std::string, Model *>::iterator it2= m_models.begin(); it2 != m_models.end();it2++)
          it2->second->render(shadow_shader, false);

        if(m_terrain != nullptr)
          m_terrain->renderWithoutMaterial(shadow_shader);

        (*it)->unbindFromShadowRender();
      }
      if( (*it)->getType() == LIGHT_POINT )
      {        
        point_shadow_shader->use();
        for( int iteration =0; iteration <6; iteration++)
        {
          (*it)->bindForShadowRenderPoint(point_shadow_shader,iteration);
          for(std::map<std::string, Model *>::iterator it3 = m_models.begin(); it3 != m_models.end();it3++)
          {
            std::vector<Object *> objs = it3->second->getObjects();
            for(std::vector<Object *>::iterator it2 = objs.begin(); it2 != objs.end();)
            {
              glm::vec3 p = (*it2)->getMinBBDistantPoint( (*it)->getPosition() ) ;
              if(! (*it)->isInRange(p) )
                objs.erase(it2);
              else
                it2++;
            }
            it3->second->render((RenderBaseShader *)point_shadow_shader,objs, false);
          }

          if(m_terrain != nullptr)
            m_terrain->renderWithoutMaterial(point_shadow_shader);

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
  if(m_skybox != nullptr)
  {
    delete m_skybox;    
    m_skybox = nullptr;
  }

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
Object * Scene::addObject(std::string path)
{
  return addObject(path,false);
}
Object * Scene::addObjectInstanced(std::string path)
{
  return addObject(path,true);
}
Object * Scene::addObject(std::string path, glm::vec3 position, bool instanced)
{
  Model * m = RessourceManager::loadModel(path, instanced);
  Object * obj = nullptr;
  if(m != nullptr)
  {
    obj = m->getInstance();
    obj->setId(getObjectIdentification());
    obj->setPosition(position);
    m_objects.insert(m_objects.end(),obj);
    m_models[path] = m;

    m_tree_root->insert(obj);

    refreshRenderObjects();
  }

  return obj;
}
Object * Scene::addObject(std::string path,  bool instanced)
{
  Model * m = RessourceManager::loadModel(path, instanced);
  Object * obj = m->getInstance();
  obj->setId(getObjectIdentification());
  m_objects.insert(m_objects.end(),obj);
  m_models[path] = m;

  m_tree_root->insert(obj);

  refreshRenderObjects();

  return obj;
}
void Scene::removeObject(Object * obj)
{
  for(std::vector<Object *>::iterator it = m_objects.begin(); it != m_objects.end(); )
  {
    if( (*it) == obj)
    {
      m_objects.erase(it);
      Engine::getLog()->log("Scene", "removed object from scene");
    }
    else
      it++; 
  }
  refreshRenderObjects();
}

Terrain * Scene::addTerrain()
{
  if(m_terrain == nullptr)
    m_terrain = new Terrain();

  return m_terrain;
}

Light * Scene::addLight()
{
  Light * light = new Light();
  m_lights.insert(m_lights.end(), light);  
  return light; 
}

void Scene::removeLight(Light * l)
{
  int id = l->getId();
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();)
  {
    if(id == (*it)->getId())
    {
      delete (*it);
      m_lights.erase(it);
    }
    else
      it++;
  }
}

Camera * Scene::getCamera()
{
  return m_camera;
}

void Scene::cameraMoved()
{
  //std::cout << "refresh shadows "<<std::endl;
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
  {
    if( (*it)->getType() == LIGHT_DIRECTIONAL )
      (*it)->refresh();
  }
  refreshRenderObjects();
}

void Scene::refreshLights()
{
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
  {
    if( (*it)->getType() == LIGHT_DIRECTIONAL )
      (*it)->refresh();
    if( (*it)->getType() == LIGHT_SPOT )
      (*it)->refresh();
    if( (*it)->getType() == LIGHT_POINT )
      (*it)->refresh();
  }
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
    angle_mid = glm::degrees(angle_mid);    

    /*// tmp disable due to material flickers 
    if( ( ((*it)->getPriorityRender() ) || (angle_mid < m_camera->getFOV()) )  && ((*it)->isVisible())  ) //
    {
      //std::cout << angle_mid << std::endl;
      //std::cout << (*it)->getIdentifier()<<std::endl;*/
    
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
    //} //tmp disable due to flickers 
  }
  //Engine::getLog()->log("Scene", "refreshed objects");
}

SceneTreeElement * Scene::getSceneRoot()
{
  return m_tree_root;
}

int Scene::getObjectIdentification()
{
  int id = m_object_counter;
  m_object_counter++;
  return id;
}

Object * Scene::getObject(int id)
{
  for(std::vector<Object *>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
  {
    if( (*it)->getId() == id)
      return (*it);
  }
  return nullptr;
}
Light * Scene::getLight(int id)
{
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end(); it++)
  {
    if( (*it)->getId() == id)
      return (*it);
  }
  return nullptr;
}
   
void Scene::save(std::string path)
{
  tinyxml2::XMLDocument document;
  tinyxml2::XMLNode * root = document.NewElement("Scene");
  document.InsertFirstChild(root);

  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end(); it++)
  {
    (*it)->save(root, &document);
  }
  for(std::vector<Object *>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
  {
    (*it)->save(root, &document);
  }
  if(m_skybox  != nullptr)
  {
    m_skybox->save(root,&document);
  }

  tinyxml2::XMLError eResult = document.SaveFile(path.c_str());
}
bool Scene::load(std::string path)
{
  tinyxml2::XMLDocument document;
  tinyxml2::XMLError result = document.LoadFile(path.c_str() );



  if(result != tinyxml2::XML_SUCCESS)
  {
    Engine::getLog()->log("Scene", "couldn't load scene file");
    return false;
  }

  tinyxml2::XMLNode * root = document.FirstChild();

  if(root !=nullptr)
  {
    Engine::getLog()->log("Scene", "loading: ", path);
    tinyxml2::XMLElement * child = root->FirstChildElement();

    while (child != nullptr)
    {
      std::string type = std::string(child->Name() );
      Engine::getLog()->log("Scene", "element read:", type);
      if( type == "Light")
      {
        Light * new_light = addLight();
        if( ! new_light->load(child) )
          removeLight(new_light);  
      }
      if( type == "Object")
      {
        std::string file_name = "";
        tinyxml2::XMLElement * el = child->FirstChildElement("Identifier");
        if( el != nullptr)
        {
            file_name =std::string( el->Attribute("value") );
        }
        Object * new_object = addObject(file_name,glm::vec3(0,0,0) ); 
        new_object->load(child);
      }
      if( type == "Skybox")
      {
        if(m_skybox != nullptr)
          delete m_skybox;
        m_skybox = new Skybox();
        m_skybox->load(child);
      }

      child = child->NextSiblingElement();
    }

  }
  else  
    Engine::getLog()->log("Scene", "no children in scene file");    
  



  return true;
}