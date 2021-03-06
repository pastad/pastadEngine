#include "Scene.h"

#include "RessourceManager.h"
#include "RenderSubsystem.h"
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
#include "Water.h"
#include "Helper.h"
#include "SceneTreeElement.h"

#include <iostream>
#include <iomanip>
#include <future>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <numeric>

#include "tinyxml2.h"

Scene::Scene()
{
  m_object_counter = 1;
  m_camera = new Camera(0,2,0);
  m_skybox =  nullptr;
  m_terrain = nullptr;
  m_tree_root= new SceneTreeElement(200, glm::vec3(0,0,0));
  Engine::getLog()->log("Scene", "created");

  m_fog_color = glm::vec3(1,1,1);
  m_fog_factor = 0.0f;
  m_fog_offset = 0.0f;
  m_time_line_seconds = 0.0f;
  }

Scene::~Scene()
{  
  for(std::vector<Object *>::iterator it = m_objects_static.begin(); it != m_objects_static.end();it++)
  {
    if( (*it) != nullptr)
      delete (*it);
  }
  Engine::getLog()->log("Scene", "deleted all static objects");
  for(std::vector<Object *>::iterator it = m_objects_dynamic.begin(); it != m_objects_dynamic.end();it++)
  {
    if( (*it) != nullptr)
      delete (*it);
  }
  Engine::getLog()->log("Scene", "deleted all dynamic objects");
  m_objects_dynamic.clear();
  m_objects_static.clear();
  if(m_tree_root != nullptr)
    delete m_tree_root;
  Engine::getLog()->log("Scene", "deleted the tree");
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
  {
    if( (*it) != nullptr)
      delete (*it);
  }
  Engine::getLog()->log("Scene", "deleted all lights");
  m_lights.clear();
  if(m_skybox != nullptr)
    delete m_skybox;
  if(m_camera != nullptr)
    delete m_camera;
  if(m_terrain != nullptr)
    delete m_terrain;
  Engine::getLog()->log("Scene", "deletion complete");
}


//  update -------------------------------------------------

void Scene::update(float delta)
{
  m_camera->update(delta);
  //update( m_tree_root,delta);// REMARKS : for advanced stuff, not working

  timeUpdate(delta * m_time_advance);

  std::future<void> future_scriptupdate = std::async(updateObjectsScripts, m_objects_scripted, delta * m_time_advance, this);
  std::future<void> future_animateupdate = std::async(updateObjectsAnimated, m_objects_animated, delta * m_time_advance);
  future_scriptupdate.get();
  future_animateupdate.get();

}

void Scene::updateObjectsScripts(std::vector<Object *> objs, float delta, Scene * current)
{
  while( objs.size() > 0 )
  {
    for (std::vector<Object *>::iterator it = objs.begin(); it != objs.end(); )
    {
      std::cout<< "script up" <<std::endl;
      (*it)->update(delta, current);

      it = objs.erase(it);
    }
  }
}
void Scene::updateObjectsAnimated(std::vector<Object *> objs, float delta ) 
{
  while (objs.size() > 0)
  {
    for (std::vector<Object *>::iterator it = objs.begin(); it != objs.end(); )
    {
      (*it)->advanceAnimation(delta);

      it = objs.erase(it);
    }
  }
}


// maybe not relevent anymore
/*
void Scene::update(SceneTreeElement * element,  float delta)
{
  std::vector< SceneTreeElement *> children = element->getChildren();
  std::vector< Object *> objects = element->getObjects();

  //std::vector<std::future<void>> updated;
  for( int x=0;  x< objects.size(); x++)
  {    
    Object * obj =objects.at(x);
    obj->advanceAnimation(delta); 
    obj->update(delta, this);
  }
  for( int x=0;  x< children.size(); x++)
  {
    update(children.at(x), delta /2.0f);
  }

}*/

void Scene::timeUpdate(float delta)
{
  m_time_line_seconds += delta;
  //std::cout <<delta <<std::endl;
  //std::cout << getTimeString() <<std::endl;
}


//  render -------------------------------------------------

void Scene::render(RenderShader * render_shader, SkyboxShader * skybox_shader, RenderBaseShader * terrain_shader , RenderBaseShader *  water_shader, bool transparent)
{
  //acquireLock("scene render");

  render_shader->setLights(&m_lights);
  render_shader->setCameraPosition(m_camera->getPosition());
  render_shader->setFog(m_fog_color,m_fog_factor,m_fog_offset);

 // float angle_mid = glm::orientedAngle(glm::vec2(1,1),  glm::vec2( v.x ,v.z)   );

  for (std::vector<Model *>::iterator it = m_models_instanced.begin(); it != m_models_instanced.end(); it++)
  {
    (*it)->renderTransparent((RenderBaseShader *)render_shader,true, transparent);
  }

  // render the accumulated models
	for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
  {
    std::map<std::string, std::vector<Object *>>::iterator objs =m_render_objects.find(it->first);
    if(objs !=  m_render_objects.end() )
    {
      if(objs->second.size() >0  )
        it->second->render((RenderBaseShader *)render_shader ,objs->second, true, transparent, true);
    }
  }
  int c = 0;

  if(Engine::isInEditMode() || Engine::isInExternalEditMode())
  {
    for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
    {
     // (*it)->editRender(render_shader,c);
      if((*it)->getType()  == LIGHT_DIRECTIONAL)
        c++;
    }
  }

  if(m_terrain != nullptr)
    m_terrain->render(terrain_shader);

  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
  
  //glDisable(GL_BLEND);
    
  if(skybox_shader != nullptr)
    renderSkybox(skybox_shader);

 // water_shader->setUniform("delta", m_time_line_seconds);
  for(std::vector<Water *>::iterator it = m_water_effects.begin(); it != m_water_effects.end();it++)
  {
    (*it)->render(water_shader);
  }
  //releaseLock("scene render");
}

void Scene::renderShadow(RenderBaseShader * shadow_shader, RenderBaseShader* point_shadow_shader, bool directional_enabled, bool point_enabled)
{  
   // acquireLock("scene render shadow");
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();it++)
  {
    if( (*it)->getShadowRefresh() && (*it)->isShadowEnabled())
    {
      if( directional_enabled && ( (*it)->getType() == LIGHT_SPOT ) )
      {
        shadow_shader->use();
        (*it)->bindForShadowRenderDirectional(shadow_shader);
        gl::ClearColor(0, 0, 0, 0);       

        // TODO make me faster
        for (std::vector<Model *>::iterator it = m_models_instanced.begin(); it != m_models_instanced.end(); it++)
        {
          (*it)->render((RenderBaseShader *)shadow_shader, false);
        }    

        for(std::map<std::string, Model *>::iterator it2 = m_models.begin(); it2 != m_models.end();it2++)
        {
          std::vector<Object *> objs = it2->second->getObjects();
          for(std::vector<Object *>::iterator it3 = objs.begin(); it3 != objs.end();)
          {
                 
            float angle = (*it3)->getMinAngleToLight( (*it) );
            glm::vec3 p = (*it3)->getMinBBDistantPoint( (*it)->getPosition() ) ;
            //std::cout <<angle<<std::endl;
            if( false) // (angle > (*it)->getCutoffAngle() ) ||  ( (*it3)->getVisibility() != V_All) )// (! (*it)->isInRange(p) ) ||
              it3 = objs.erase(it3);
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
      if( directional_enabled && ( (*it)->getType() == LIGHT_DIRECTIONAL ) )
      {             
        shadow_shader->use();
        (*it)->bindForShadowRenderDirectional(shadow_shader);
        gl::ClearColor(0, 0, 0, 0);

        // TODO make me faster
        for (std::vector<Model *>::iterator it = m_models_instanced.begin(); it != m_models_instanced.end(); it++)
        {
          (*it)->render((RenderBaseShader *)shadow_shader, false);
        }

        for(std::map<std::string, Model *>::iterator it2= m_models.begin(); it2 != m_models.end();it2++)
          it2->second->render(shadow_shader, false);

        if(m_terrain != nullptr)
          m_terrain->renderWithoutMaterial(shadow_shader);

        (*it)->unbindFromShadowRender();
      }
      if( point_enabled && ( (*it)->getType() == LIGHT_POINT ) )
      {        
        point_shadow_shader->use();
        for( int iteration =0; iteration <6; iteration++)
        {
          gl::ClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
          (*it)->bindForShadowRenderPoint(point_shadow_shader,iteration);
        

          // TODO make me faster
          for (std::vector<Model *>::iterator it = m_models_instanced.begin(); it != m_models_instanced.end(); it++)
          {
            (*it)->render((RenderBaseShader *)shadow_shader, false);
          }

          for(std::map<std::string, Model *>::iterator it3 = m_models.begin(); it3 != m_models.end();it3++)
          {
            std::vector<Object *> objs = it3->second->getObjects();
            for(std::vector<Object *>::iterator it2 = objs.begin(); it2 != objs.end();)
            {
              glm::vec3 p = (*it2)->getMinBBDistantPoint( (*it)->getPosition() ) ;
              if( ( (*it2)->getVisibility() == V_None ) ) //! (*it)->isInRange(p) ||
                it2 = objs.erase(it2);
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
  //releaseLock("scene render shadow");
}

void Scene::renderSkybox(SkyboxShader * skybox_shader)
{
  skybox_shader->use();

  skybox_shader->setView(m_camera->getViewWithoutTranslation());
  skybox_shader->setProjection(m_camera->getProjection());

  if(m_skybox != nullptr)
    m_skybox->render(skybox_shader);
}


//  refreshers -------------------------------------------------

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
  //  std::cout << "refresh lights "<<std::endl;
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

void Scene::refreshRenderObjectsSceneTree()
{
  m_render_objects.clear();
  glm::vec3 v = m_camera->getDirection();
  v = glm::normalize(v);
  m_tree_root->getObjectsInFrustrum(m_camera,& m_render_objects,false);
}

void Scene::refreshRenderObjects()
{
  m_render_objects.clear();
  for(std::vector<Object *>::iterator ito = m_objects_static.begin(); ito != m_objects_static.end();ito++)
  {
    (*ito)->unsetExtractionFlag();
  }
  //refreshRenderObjectsSceneTree(); // REMARKS: again advanced
  //std::cout << m_render_objects.size()<<std::endl;
  //m_render_objects.clear();

  glm::vec3 v = m_camera->getDirection();
  v = glm::normalize(v);

  for(std::vector<Object *>::iterator it = m_objects_dynamic.begin(); it != m_objects_dynamic.end(); it++)
  {
    glm::vec3 dir_to_object  = (*it)->getPosition() - m_camera->getPosition();
    //float angle_mid = std::acos(glm::dot( glm::vec2(dir_to_object.x,dir_to_object.z), glm::vec2( v.x ,v.z) ) / ( glm::length(glm::vec2(dir_to_object.x,dir_to_object.z)) * glm::length(glm::vec2( v.x ,v.z) )  )); 
   // angle_mid = glm::degrees(angle_mid);  
   // float angle_min = (*it)->getMinAngleToCamera(m_camera);  

    // tmp disable due to material flickers 
    if( ( ((*it)->getPriorityRender() ) || m_camera->insideFrustrum((*it)) )  && (  (*it)->getVisibility() == V_All) ) //(angle_min < m_camera->getFOV())
    {
      //std::cout << angle_mid << std::endl;
      //std::cout << (*it)->getIdentifier()<<std::endl;
    
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
    } //tmp disable due to flickers 
  }

  // QUICKFIX ->move to scene tree when debugged
  for (std::vector<Object *>::iterator it = m_objects_static.begin(); it != m_objects_static.end(); it++)
  {
	  glm::vec3 dir_to_object = (*it)->getPosition() - m_camera->getPosition();

	  if ((((*it)->getPriorityRender()) || m_camera->insideFrustrum((*it))) && ((*it)->getVisibility() == V_All) ) //(angle_min < m_camera->getFOV())
	  {
		  std::map< std::string, std::vector<Object *> >::iterator entry = m_render_objects.find((*it)->getIdentifier());
		  if (entry != m_render_objects.end())
		  {
			  entry->second.insert(entry->second.end(), (*it));
		  }
		  else
		  {
			  std::vector<Object *> ins_vec;
			  ins_vec.insert(ins_vec.end(), (*it));
			  m_render_objects.insert(std::pair< std::string, std::vector<Object *> >((*it)->getIdentifier(), ins_vec));
		  }
	  }
  }
  // QUICKFIX EnD
  //Engine::getLog()->log("Scene", "refreshed objects");*/
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

   

//  load/save -------------------------------------------------

void Scene::save(std::string path)
{
  tinyxml2::XMLDocument document;
  tinyxml2::XMLNode * root = document.NewElement("Scene");
  document.InsertFirstChild(root);

  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end(); it++)
  {
    (*it)->save(root, &document);
  }
  for(std::vector<Object *>::iterator it = m_objects_static.begin(); it != m_objects_static.end(); it++)
  {
    (*it)->save(root, &document);
  }
  for(std::vector<Object *>::iterator it = m_objects_dynamic.begin(); it != m_objects_dynamic.end(); it++)
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
 // acquireLock();

  tinyxml2::XMLDocument document;
  tinyxml2::XMLError result = document.LoadFile(path.c_str() );
  
  if(result != tinyxml2::XML_SUCCESS)
  {
    Engine::getLog()->log("Scene", "couldn't load scene file");
    releaseLock("load");
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
		    if (new_light != nullptr)
		    {
          Engine::getLog()->log("Scene", "adding light to scene");
			    if (!new_light->load(child))
				    removeLight(new_light);
          Engine::getLog()->log("Scene", "Light added");
		    }
      }
      if( type == "Object")
      {
        std::string file_name = "";
        bool static_ob = false ;
        tinyxml2::XMLElement * el = child->FirstChildElement("Identifier");
        if (el != nullptr)
        {
          file_name = std::string(el->Attribute("value"));
        }
        
        el = child->FirstChildElement("Static");
        if( el != nullptr)
        {
          static_ob =el->BoolAttribute("value") ;
        }
        Object * new_object = addObject(file_name,glm::vec3(0,0,0), false , false, static_ob); 
    

        if (new_object == nullptr)
        {
          removeObject(new_object);
        }
        else
        {
          if (!new_object->load(child))
          {
            removeObject(new_object);
          }
          else
          {
            new_object->refreshAABB();
            m_tree_root->insert(new_object);
          }
        }
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
  Helper::checkGLError("SceneLoad");

  return true;
}



//  getter/setter -------------------------------------------------

//time

float Scene::getTime()
{
  return m_time_line_seconds;
}

std::string Scene::getTimeString()
{
  std::stringstream ss;
  float mins = (int)m_time_line_seconds / 60;
  float secs = (int)m_time_line_seconds % 60;
  ss << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) <<secs;
  return ss.str();
}

void Scene::setTime(float time)
{
  m_time_line_seconds  = time;
}

//root

SceneTreeElement * Scene::getSceneRoot()
{
  return m_tree_root;
}

//light

Light * Scene::addLight()
{ 
  acquireLock("add Light ");
  Light * light = new Light();
  m_lights.insert(m_lights.end(), light);  
  releaseLock("add Light");
  return light; 
}

void Scene::removeLight(Light * l)
{
  acquireLock("remove Light ");
  int id = l->getId();
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end();)
  {
    if(id == (*it)->getId())
    {
      delete (*it);
      m_lights.erase(it);
	  it = m_lights.end();
    }
    else
      it++;
  }
  releaseLock("remove Light");
}

Light * Scene::getLight(int id)
{
  acquireLock("add light");
  for(std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end(); it++)
  {
    if( (*it)->getId() == id)
      return (*it);
  }
  releaseLock("add light");
  return nullptr;
}
std::vector<Light *> Scene::getLights()
{
  return m_lights;
}

//object

Object * Scene::addObject(std::string path, glm::vec3 position, bool static_object)
{
  return addObject(path,position,false,true, static_object);
}
Object * Scene::addObjectInstanced(std::string path, glm::vec3 position, bool static_object)
{
  return addObject(path,position,true,true,static_object);
}

Object * Scene::addObject(std::string path, bool static_object)
{
  return addObject(path,false, static_object);
}
Object * Scene::addObjectInstanced(std::string path, bool static_object)
{
  return addObject(path,true, static_object);
}

// scripted

void Scene::objectIsScripted(Object * obj)
{
  std::vector<Object *>::iterator it =std::find(m_objects_scripted.begin(), m_objects_scripted.end(), obj);
  if(it == m_objects_scripted.end())
    m_objects_scripted.push_back(obj);
}

// privates
Object * Scene::addObject(std::string path, glm::vec3 position, bool instanced, bool insert_in_tree, bool static_object)
{
  try
  {
    acquireLock("scene add");
    Helper::checkGLError("add Object");

    Engine::getLog()->log("Scene", "adding Object");
    Model * m = RessourceManager::loadModel(path, instanced);
    Object * obj = nullptr;
    if(m != nullptr)
    {
      obj = m->getInstance(this);
      if( obj != nullptr)
      {
            Engine::getLog()->log("Scene", "adding Object2");
            std::cout << obj <<std::endl;
            std::cout << obj->getModel() <<std::endl;
    //    obj->setId(getObjectIdentification());
        obj->setPosition(position);
        if (obj->getModel()->isInstanced())
        {
              Engine::getLog()->log("Scene", "adding Object3i");
          bool should_be_inserted = true;
          for (int x = 0; x < m_models_instanced.size(); x++)
          {
            if(m_models_instanced[x] == obj->getModel() )
              should_be_inserted = false;
          }
          if(should_be_inserted)
            m_models_instanced.push_back(obj->getModel() );
        }
        else
        {
              Engine::getLog()->log("Scene", "adding Object3");
          if( obj->isAnimated())
            m_objects_animated.push_back(obj);

          if(static_object && (! Engine::isInEditMode()) && (!obj->isAnimated()) )
          {
            m_objects_static.push_back(obj);
            obj->setStaticFlag();   
          }
          else
            m_objects_dynamic.push_back(obj);
          m_models[path] = m;
                          std::cout << obj <<std::endl;
        //  m_models.insert(std::pair<std::string , Model*>(path, m) );
        //  if(insert_in_tree )
        //    m_tree_root->insert(obj);
          refreshRenderObjects();
        }
      }
    }
    else    
       Engine::getLog()->log("Scene", "Object is null");
    Engine::getLog()->log("Scene", "done");
    releaseLock("scene add");
    return obj;
  }
  catch (std::exception ex)
  {
    Engine::getLog()->log("Scene", ex.what());
  }
  return nullptr;
}
Object * Scene::addObject(std::string path,  bool instanced ,  bool static_object)
{
  Engine::getLog()->log("Scene", "adding Object");
  Model * m = RessourceManager::loadModel(path, instanced);
  Object * obj  = nullptr;
  if( m != nullptr)
  {
    obj = m->getInstance(this);
    obj->setId(getObjectIdentification());

    if (obj->getModel()->isInstanced())
    {
      bool should_be_inserted = true;
      for (int x = 0; x < m_models_instanced.size(); x++)
      {
        if (m_models_instanced[x] == obj->getModel())
          should_be_inserted = false;
      }
      if (should_be_inserted)
        m_models_instanced.push_back(obj->getModel());
    }
    else
    {
      if(static_object && (! Engine::isInEditMode()) && (!obj->isAnimated()) )
      {
        obj->setStaticFlag();
        m_objects_static.push_back(obj);
        m_tree_root->insert(obj);
      }
      else
        m_objects_dynamic.push_back(obj);
      //m_objects.insert(m_objects.end(),obj);
      m_models[path] = m;
    }

    refreshRenderObjects();
  }

  return obj;
}

void Scene::removeObject(Object * obj)
{
  if(obj == nullptr)
    return;
  if( obj->isStaticFlagSet() )
  {
    std::cout << obj << " " << obj->isStaticFlagSet() <<" "<< m_objects_static.size() << std::endl;
    for(std::vector<Object *>::iterator it = m_objects_static.begin(); it != m_objects_static.end(); )
    {
      std::cout << (*it)<< std::endl;
      if( (*it) == obj)
      {
        m_objects_static.erase(it);
        Engine::getLog()->log("Scene", "removed static object from scene");
	      it = m_objects_static.end();
      }
      else
        it++; 
    }
  }
  else
  {
    for(std::vector<Object *>::iterator it = m_objects_dynamic.begin(); it != m_objects_dynamic.end(); )
    {
      if( (*it) == obj)
      {
        m_objects_dynamic.erase(it);
        Engine::getLog()->log("Scene", "removed dynamic object from scene");
	    it = m_objects_dynamic.end();
      }
      else
        it++; 
    }
  }
  if (obj->isAnimated())
  {
    for (std::vector<Object *>::iterator it = m_objects_animated.begin(); it != m_objects_animated.end(); )
    {
      if ((*it) == obj)
      {
        m_objects_animated.erase(it);
        Engine::getLog()->log("Scene", "removed animated object from scene");
        it = m_objects_animated.end();
      }
      else
        it++;
    }
  }
  for (std::vector<Object *>::iterator it = m_objects_scripted.begin(); it != m_objects_scripted.end(); )
  {
    if ((*it) == obj)
    {
      m_objects_scripted.erase(it);
      Engine::getLog()->log("Scene", "removed scripted object from scene");
      it = m_objects_scripted.end();
    }
    else
      it++;
  }

  if (obj != nullptr)
  {
    m_tree_root->remove(obj);
    obj->getModel()->removeInstance(obj);
    refreshRenderObjects();
  }
}

std::vector<Object *> Scene::getPhysicsStaticObjects()
{  
  std::vector<Object *> objs;
  for(std::vector<Object *>::iterator it = m_objects_static.begin(); it != m_objects_static.end(); it++)
  {
    if( (*it)->isPhysicsApplied() && (*it)->isPhysicsStatic() )
      objs.push_back( (*it) );
  }
  for(std::vector<Object *>::iterator it = m_objects_dynamic.begin(); it != m_objects_dynamic.end(); it++)
  {
    if( (*it)->isPhysicsApplied() && (*it)->isPhysicsStatic() )
      objs.push_back( (*it) );
  }
  return objs;
}

std::vector<Object *> Scene::getObjects(std::string ident)
{
  std::vector<Object *> objs;
  for(std::vector<Object *>::iterator it = m_objects_static.begin(); it != m_objects_static.end(); it++)
  {
    if( (*it)->getIdentifier() == ident)
      objs.push_back( (*it) );
  }
  for(std::vector<Object *>::iterator it = m_objects_dynamic.begin(); it != m_objects_dynamic.end(); it++)
  {
    if(  (*it)->getIdentifier() == ident)
      objs.push_back( (*it) );
  }
  return objs;
}

std::vector<Object *> Scene::getObjects()
{
  std::vector<Object *> objs = m_objects_static;
  objs.insert(std::end(objs), std::begin(m_objects_dynamic), std::end(m_objects_dynamic));
  return objs;
}


Object * Scene::getObject(int id)
{
  for(std::vector<Object *>::iterator it = m_objects_static.begin(); it != m_objects_static.end(); it++)
  {
    if( (*it)->getId() == id)
      return (*it);
  }
  for(std::vector<Object *>::iterator it = m_objects_dynamic.begin(); it != m_objects_dynamic.end(); it++)
  {
    if( (*it)->getId() == id)
      return (*it);
  }
  return nullptr;
}

int Scene::getObjectIdentification()
{
  int id = m_object_counter;
  m_object_counter++;
  return id;
}

// water

Water * Scene::addWaterEffect(glm::vec3 pos, float size)
{
  Water * w = new Water(pos,size);
  if(!w->initialize())
  {
    delete w;
    return nullptr;
  }
  m_water_effects.push_back(w);
  return w;  
}

// terrain

Terrain * Scene::addTerrain()
{
  if(m_terrain == nullptr)
    m_terrain = new Terrain();

  return m_terrain;
}

// skybox

Skybox *  Scene::setSkybox(const std::string path)
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
    return m_skybox;  
  }

  return m_skybox;
}

//camera

Camera * Scene::getCamera()
{
  return m_camera;
}


// fog

void Scene::setFog(glm::vec3 color, float factor, float offset)
{
  m_fog_color = color;
  m_fog_factor = factor;
  m_fog_offset = offset;
}

// time 

void Scene::setTimeAdvance(float speed)
{
  m_time_advance = speed;
}

// locking

void Scene::acquireLock(std::string who)
{
  try
  {
    Engine::getLog()->log(LF_TS, "Scene", "lock wanted by ", who);
    m_mutex.lock();
    Engine::getLog()->log(LF_TS, "Scene", "lock acquired by ", who);
  }
  catch (std::exception ex)
  {
    Engine::getLog()->log(LF_TS, "EXCEPTION", "locking");
  }
}

void Scene::releaseLock(std::string who)
{
  try
  {
    m_mutex.unlock();
    Engine::getLog()->log(LF_TS, "Scene", "lock unlocked by ", who);
  }
  catch (std::exception ex)
  {
    Engine::getLog()->log(LF_TS, "EXCEPTION", "locking");
  }
}

float Scene::getTimeAdvance()
{
  return m_time_advance;
}
