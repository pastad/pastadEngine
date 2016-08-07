#include "EnergyShot.h"

#include "Mobs.h"
#include "Mob.h"
#include "Environment.h"

#include "Object.h"
#include "Scene.h"
#include "Model.h"
#include "Material.h"

#include <iostream>
#include <map>

#include "Script.h"
#include "RotationScriptElement.h"

#define MOVEMENT_SPEED 5

#define MAX_SCALE 2.0f
#define MAX_DAMAGE 25.0f
#define DAMAGE_DRAIN 5.0f

EnergyShot::EnergyShot()
{  
  m_target = nullptr;
}
EnergyShot::~EnergyShot()
{ 
}

bool EnergyShot::initialize(Scene * scene, Mob * target, glm::vec3 start_position, float damage)
{
  m_object = scene->addObject("game/models/energy_remain.obj",start_position, false);
  std::map<int, Material * > materials =  m_object->getModel()->getMaterials();
  for(std::map<int,Material*>::iterator it = materials.begin(); it != materials.end();it++)
  { 
    it->second->setEmmissive(1.0f);
  }             
  Script * script = m_object->addScript();
  RotationScriptElement * rse = script->addRotationScript();
  rse->setupAlways();
  rse->setup(glm::vec3(1000.0f,1000.0f,1000.0f));

  m_target = target;
  m_damage = damage;
  //m_object->setRotation(glm::vec3(0.0f,90.0f,90.0f));  
  if(m_object !=nullptr)
    return true;
  else
    return false;
}

bool EnergyShot::update(float delta, Mobs * mobs, bool * mob_killed, Environment * env)
{
  *mob_killed = false;

  m_damage -= DAMAGE_DRAIN*delta;

  // deleteit if damage < 0 otherwise adjust scale
  if(m_damage <=0.0f)
    return true;
  else
  {
    float s = ( MAX_SCALE / MAX_DAMAGE ) * m_damage;
    m_object->setScale(glm::vec3(s,s,s));
  }

  if(m_target!=nullptr)
  {
    glm::vec3 dir = glm::normalize( m_target->getObject()->getPosition() - m_object->getPosition() );
    float distance = glm::distance(m_target->getObject()->getPosition() , m_object->getPosition());   

    // if we are too far away, move closer | othwerwise do damage to our target ..
    if( distance > 0.1f)
    {
      dir *= MOVEMENT_SPEED* delta;
      m_object->setPosition( m_object->getPosition() +dir );
    }
    else
    {
      // ... and remove it if it is dead
      if(m_target->doDamage(m_damage) )
      {
        mobs->removeMob(m_target, env);      
        *mob_killed = true;
      }

      return true;
    }
    return false;
  }
  return false;
}

void EnergyShot::changeTarget(Mob * old_target, Mobs * mobs)
{
  if( m_target == old_target)
  {
    Mob * nt = mobs->getClosestMobInRange(m_object->getPosition(), 1000.0f);
    m_target = nt;    
  }
}
Mob * EnergyShot::getTarget()
{
  return m_target;
}
Object * EnergyShot::getObject()
{
  return m_object;
}