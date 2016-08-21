#include "EnergySpark.h"

#include "Object.h"
#include "Scene.h"
#include "Model.h"
#include "Material.h"

#include <iostream>
#include <map>

#define SPEED 3.0

EnergySpark::EnergySpark()
{  
}
EnergySpark::~EnergySpark()
{ 
}

bool EnergySpark::initialize(Scene * scene, glm::vec3 start_position, glm::vec3 target_position, unsigned int plant_type)
{
  m_object = scene->addObject("game/models/energy_remain.obj",start_position, false);
  std::map<int, Material * > materials =  m_object->getModel()->getMaterials();
  for(std::map<int,Material*>::iterator it = materials.begin(); it != materials.end();it++)
  { 
    it->second->setEmmissive(1.0f);
  }      

  m_target_position = target_position;      
  m_plant_type = plant_type; 

  if(m_object !=nullptr)
    return true;
  else
    return false;
}

bool EnergySpark::initialize(Object * object,  glm::vec3 target_position, unsigned int plant_type)
{
  m_object = object;
  m_target_position = target_position;
  m_plant_type = plant_type;
  return true;
}

bool EnergySpark::update(float delta)
{
  float step = delta * SPEED;
  glm::vec3 d = glm::normalize( m_target_position - m_object->getPosition() );

  if (glm::distance(m_target_position, m_object->getPosition()) <= step)
	  return true;

  m_object->setPosition( m_object->getPosition() + d * step );


  return false;
}

Object * EnergySpark::getObject()
{
  return m_object;
}

unsigned int EnergySpark::getPlantType()
{
  return m_plant_type;
}

glm::vec3 EnergySpark::getTarget()
{
  return m_target_position;
}