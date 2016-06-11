#include "SceneTreeElement.h"

#include "Object.h"
#include "Engine.h"
#include "Log.h"
#include "BoundingBox.h"
#include "Camera.h"

#include <glm/gtx/vector_angle.hpp>
#include "glm/ext.hpp"
#include <iostream>

SceneTreeElement::SceneTreeElement(unsigned int size, glm::vec3 center):m_size(size),m_center(center)
{  
  std::cout << m_center.x <<std::endl;
  glm::vec3 min = center - glm::vec3((float)size/2.0,(float)size/2.0,(float)size/2.0);
  glm::vec3 max = center + glm::vec3((float)size/2.0,(float)size/2.0,(float)size/2.0);
  m_aabb = new BoundingBox(min.x,max.x,min.y,max.y,min.z,max.z);
}

SceneTreeElement::~SceneTreeElement()
{
  if(m_top_front_left != nullptr)
    delete m_top_front_left;
  if(m_top_front_right != nullptr)
    delete m_top_front_right;
  if(m_top_back_left != nullptr)
    delete m_top_back_left;
  if(m_top_back_right != nullptr)
    delete m_top_back_right;

  if(m_bottom_front_left != nullptr)
    delete m_bottom_front_left;
  if(m_bottom_front_right != nullptr)
    delete m_bottom_front_right;
  if(m_bottom_back_left != nullptr)
    delete m_bottom_back_left;
  if(m_bottom_back_right != nullptr)
    delete m_bottom_back_right;

  if(m_aabb!=nullptr)
    delete m_aabb;
}

bool SceneTreeElement::inside(glm::vec3 pos)
{
  // TODO : implement ME
  return false;
}

void SceneTreeElement::insert( Object * obj)
{
  glm::vec3 delta = obj->getPosition() -  m_center;
  if( (m_size <= STOP_SIZE)  )
  {
    m_objects.insert(m_objects.end(), obj);
    Engine::getLog()->log("SceneTreeElement", "inserted object");
  //  std::cout << m_center.x << ","<<m_center.y<<","<<m_center.z <<std::endl;
  //  std::cout << obj->getPosition().x << ","<<obj->getPosition().y<<","<<obj->getPosition().z <<std::endl;
    m_last_element = true;
  }
  else
  {
    float new_size = (float)m_size / 4.0f ;

    // insert according to where the obj in relation to the center
    insert(&m_top_front_left,obj,   m_center + glm::vec3(new_size,new_size,new_size) );      
    insert(&m_top_front_right,obj,   m_center + glm::vec3(-new_size,new_size,new_size));      
    insert(&m_top_back_left,obj ,   m_center + glm::vec3(new_size,-new_size,new_size) );
    insert(&m_top_back_right,obj,   m_center + glm::vec3(-new_size,-new_size,new_size));
    insert(&m_bottom_front_left,obj,  m_center + glm::vec3(new_size,new_size,-new_size) );
    insert(&m_bottom_front_right,obj,  m_center + glm::vec3(-new_size,new_size,-new_size));
    insert(&m_bottom_back_left,obj,  m_center + glm::vec3(new_size,-new_size,-new_size));
    insert(&m_bottom_back_right,obj,  m_center + glm::vec3(-new_size,-new_size,-new_size));
      
  }  
}

void SceneTreeElement::insert(SceneTreeElement ** se , Object * obj, glm::vec3 new_center)
{

  if(*se == nullptr)
  {
    //std::cout << "new scte"<<std::endl;
    (*se) = new SceneTreeElement( (float)m_size/2.0, new_center);
  }
  if( (*se)->getBoundingBox()->intersectAABBs(obj->getAABB())  )
  {
  //  std::cout << "insert called"<<std::endl;
    (*se)->insert(obj);
  }
  else
  {
  //  std::cout << "not in it"<<std::endl;
  }

}

void SceneTreeElement::getObjectsInFrustrum(Camera* cam, std::map<std::string, std::vector<Object *>> * objects, bool fully_inside)
{
  if(m_last_element)
  {

    for(std::vector<Object *>::iterator ito = m_objects.begin(); ito != m_objects.end();ito++)
    {
      Object * ob = (*ito);
      if( ! ob->isExtractionFlagSet() )
      {
        std::map<std::string, std::vector<Object *>>::iterator it = objects->find(ob->getIdentifier());
        if(it != objects->end() )
        {
          it->second.push_back(ob);
          ob->setExtractionFlag();
        }
        else
        {
          std::vector<Object *> vec;
          vec.push_back(ob);
          ob->setExtractionFlag();
          objects->insert( std::pair<std::string, std::vector<Object *>>(ob->getIdentifier(),vec  )  );

        }
      }
    }
  }
  else
  {  
    if(!  fully_inside) 
    {
      if( m_top_front_left != nullptr)
      {
        int res = cam->insideFrustrum(m_top_front_left->getBoundingBox());
        if(res == FRUSTRUM_INTERSECTION)
          m_top_front_left->getObjectsInFrustrum(cam,objects,false);
        if(res == FRUSTRUM_INCLUTION)
          m_top_front_left->getObjectsInFrustrum(cam,objects,true);
      }
      if( m_top_front_right != nullptr)
      {
        int res = cam->insideFrustrum(m_top_front_right->getBoundingBox());
        if(res == FRUSTRUM_INTERSECTION)
          m_top_front_right->getObjectsInFrustrum(cam,objects,false);
        if(res == FRUSTRUM_INCLUTION)
          m_top_front_right->getObjectsInFrustrum(cam,objects,true);
      }
      if( m_top_back_left != nullptr)
      {
        int res = cam->insideFrustrum(m_top_back_left->getBoundingBox());       
        if(res == FRUSTRUM_INTERSECTION)
          m_top_back_left->getObjectsInFrustrum(cam,objects,false);
        if(res == FRUSTRUM_INCLUTION)
          m_top_back_left->getObjectsInFrustrum(cam,objects,true);
      }
      if( m_top_back_right != nullptr)
      {
        int res = cam->insideFrustrum(m_top_back_right->getBoundingBox());  
        if(res == FRUSTRUM_INTERSECTION)
          m_top_back_right->getObjectsInFrustrum(cam,objects,false);
        if(res == FRUSTRUM_INCLUTION)
          m_top_back_right->getObjectsInFrustrum(cam,objects,true);
      }
      if( m_bottom_front_left != nullptr)
      {      
        int res = cam->insideFrustrum(m_bottom_front_left->getBoundingBox());  
        if(res == FRUSTRUM_INTERSECTION)
          m_bottom_front_left->getObjectsInFrustrum(cam,objects,false);
        if(res == FRUSTRUM_INCLUTION)
          m_bottom_front_left->getObjectsInFrustrum(cam,objects,true);
      }
      if( m_bottom_front_right != nullptr)
      {
        int res = cam->insideFrustrum(m_bottom_front_right->getBoundingBox());  
        if(res == FRUSTRUM_INTERSECTION)
          m_bottom_front_right->getObjectsInFrustrum(cam,objects,false);
        if(res == FRUSTRUM_INCLUTION)
          m_bottom_front_right->getObjectsInFrustrum(cam,objects,true);
      }
      if( m_bottom_back_left != nullptr)
      {
        int res = cam->insideFrustrum(m_bottom_back_left->getBoundingBox());  
        if(res == FRUSTRUM_INTERSECTION)
          m_bottom_back_left->getObjectsInFrustrum(cam,objects,false);
        if(res == FRUSTRUM_INCLUTION)
          m_bottom_back_left->getObjectsInFrustrum(cam,objects,true);
      }
      if( m_bottom_back_right != nullptr)
      {
        int res = cam->insideFrustrum(m_bottom_back_right->getBoundingBox());  
        if(res == FRUSTRUM_INTERSECTION)
          m_bottom_back_right->getObjectsInFrustrum(cam,objects,false);
        if(res == FRUSTRUM_INCLUTION)
          m_bottom_back_right->getObjectsInFrustrum(cam,objects,true);
      }
    }
    else
    {
      if( m_top_front_left != nullptr)
        m_top_front_left->getObjectsInFrustrum(cam,objects,true);
      if( m_top_front_right != nullptr)
        m_top_front_right->getObjectsInFrustrum(cam,objects,true);
      if( m_top_back_left != nullptr)
        m_top_back_left->getObjectsInFrustrum(cam,objects,true);
      if( m_top_back_right != nullptr)
        m_top_back_right->getObjectsInFrustrum(cam,objects,true);
      if( m_bottom_front_left != nullptr)
        m_bottom_front_left->getObjectsInFrustrum(cam,objects,true);
      if( m_bottom_front_right != nullptr)
        m_bottom_front_right->getObjectsInFrustrum(cam,objects,true);
      if( m_bottom_back_left != nullptr)
        m_bottom_back_left->getObjectsInFrustrum(cam,objects,true);
      if( m_bottom_back_right != nullptr)
        m_bottom_back_right->getObjectsInFrustrum(cam,objects,true);
    }
    
  }

}

void SceneTreeElement::getObjects(glm::vec3 spos, float angle_s, glm::vec3 dir, std::map< std::string , std::vector<Object *> > * object_list)
{
  // check if SceenTreeElement should be rendered
  glm::vec3 dir_to_object  = m_center - spos;
  float angle_mid = std::acos(glm::dot( glm::vec2(dir_to_object.x,dir_to_object.z), glm::vec2( dir.x ,dir.z) ) / ( glm::length(glm::vec2(dir_to_object.x,dir_to_object.z)) * glm::length(glm::vec2( dir.x ,dir.z) )  )); 
  //std::cout <<glm::degrees(angle_mid) <<std::endl;
  if(glm::degrees(angle_mid) < angle_s)
  {
    if(m_size <= STOP_SIZE)
    {
      for(std::vector<Object *>::iterator it = m_objects.begin(); it != m_objects.end();it++)
      {
        if( !(*it)->getPriorityRender()  )
        {
          std::map< std::string , std::vector<Object *> >::iterator entry = object_list->find( (*it)->getIdentifier() );
          if(entry != object_list->end())
          {
            entry->second.insert(entry->second.end(), (*it));
          }
          else
          {
            std::vector<Object *> ins_vec;
            ins_vec.insert(ins_vec.end(), (*it) );
            object_list->insert(std::pair< std::string , std::vector<Object *> >((*it)->getIdentifier(), ins_vec) );
          }
        }
      }
    }
    else
    {
      if(m_top_front_left != nullptr)
        m_top_front_left->getObjects(spos,angle_s,dir,object_list);
      if(m_top_front_right != nullptr)
        m_top_front_right->getObjects(spos,angle_s,dir,object_list);
      if(m_top_back_left != nullptr)
        m_top_back_left->getObjects(spos,angle_s,dir,object_list);
      if(m_top_back_right != nullptr)
        m_top_back_right->getObjects(spos,angle_s,dir,object_list);

      if(m_bottom_front_left != nullptr)
        m_bottom_front_left->getObjects(spos,angle_s,dir,object_list);
      if(m_bottom_front_right != nullptr)
        m_bottom_front_right->getObjects(spos,angle_s,dir,object_list);
      if(m_bottom_back_left != nullptr)
        m_bottom_back_left->getObjects(spos,angle_s,dir,object_list);
      if(m_bottom_back_right != nullptr)
        m_bottom_back_right->getObjects(spos,angle_s,dir,object_list);
    }
  }
}
std::vector<SceneTreeElement *> SceneTreeElement::getChildren()
{
  std::vector<SceneTreeElement *> ret;
  if(m_top_front_left != nullptr)
    ret.insert(ret.end(),m_top_front_left);
  if(m_top_front_right != nullptr)
    ret.insert(ret.end(),m_top_front_right);
  if(m_top_back_left != nullptr)
    ret.insert(ret.end(),m_top_back_left);
  if(m_top_back_right != nullptr)
    ret.insert(ret.end(),m_top_back_right);

  if(m_bottom_front_left != nullptr)
    ret.insert(ret.end(),m_bottom_front_left);
  if(m_bottom_front_right != nullptr)
    ret.insert(ret.end(),m_bottom_front_right);
  if(m_bottom_back_left != nullptr)
    ret.insert(ret.end(),m_bottom_back_left);
  if(m_bottom_back_right != nullptr)
    ret.insert(ret.end(),m_bottom_back_right);

  return ret;
}

std::vector<Object *> SceneTreeElement::getObjects()
{
  return m_objects;
}
BoundingBox * SceneTreeElement::getBoundingBox()
{
  return m_aabb;
}