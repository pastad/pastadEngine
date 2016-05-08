#include "SceneTreeElement.h"

#include "Object.h"
#include "Engine.h"
#include "Log.h"

#include <glm/gtx/vector_angle.hpp>
#include "glm/ext.hpp"
#include <iostream>

SceneTreeElement::SceneTreeElement(unsigned int size, glm::vec3 center):m_size(size),m_center(center)
{  
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
}

bool SceneTreeElement::inside(glm::vec3 pos)
{
  // TODO : implement ME
  return false;
}

void SceneTreeElement::insert( Object * obj)
{
  glm::vec3 delta = obj->getPosition() -  m_center;
  if(m_size <= STOP_SIZE)
  {
    m_objects.insert(m_objects.end(), obj);
    Engine::getLog()->log("SceneTreeElement", "inserted object");
  }
  else
  {
    unsigned int new_size = m_size / 2 ;

    // insert according to where the obj in relation to the center
    if(delta.z >= 0)
    {
      if(delta.y >= 0)
      {
        if(delta.x >= 0)
          insert(&m_top_front_left,obj,   m_center + glm::vec3(new_size,new_size,new_size) );      
        else      
          insert(&m_top_front_right,obj,   m_center + glm::vec3(-new_size,new_size,new_size));      
      }
      else
      {
        if(delta.x >= 0)    
          insert(&m_top_back_left,obj ,   m_center + glm::vec3(new_size,-new_size,new_size) );
        else
          insert(&m_top_back_right,obj,   m_center + glm::vec3(-new_size,-new_size,new_size));
      }
    }
    else
    {
      if(delta.y >= 0)
      {
        if(delta.x >= 0)
          insert(&m_bottom_front_left,obj,  m_center + glm::vec3(new_size,new_size,-new_size) );
        else
          insert(&m_bottom_front_right,obj,  m_center + glm::vec3(-new_size,new_size,-new_size));
      }
      else
      {
        if(delta.x >= 0)
          insert(&m_bottom_back_left,obj,  m_center + glm::vec3(new_size,-new_size,-new_size));
        else
          insert(&m_bottom_back_right,obj,  m_center + glm::vec3(-new_size,-new_size,-new_size));
      }
    }
  }
}

void SceneTreeElement::insert(SceneTreeElement ** se , Object * obj, glm::vec3 new_center)
{
  if(*se == nullptr)
  {
    (*se) = new SceneTreeElement(m_size/2, new_center);
  }
  (*se)->insert(obj);
}

void SceneTreeElement::getObjects(glm::vec3 spos, float angle_s, glm::vec3 dir, std::map< std::string , std::vector<Object *> > * object_list)
{
  // check if SceenTreeElement should be rendered
  glm::vec3 dir_to_object  = m_center - spos;
  float angle_mid = std::acos(glm::dot( glm::vec2(dir_to_object.x,dir_to_object.z), glm::vec2( dir.x ,dir.z) ) / ( glm::length(glm::vec2(dir_to_object.x,dir_to_object.z)) * glm::length(glm::vec2( dir.x ,dir.z) )  )); 
  std::cout <<glm::degrees(angle_mid) <<std::endl;
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