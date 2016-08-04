#include "EnergyShot.h"

#include "Mobs.h"
#include "Mob.h"
#include "Environment.h"

#include "Object.h"
#include "Scene.h"

#include <iostream>

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
  if(m_target == nullptr)
    std::cout <<"whooooopsi" <<std::endl;
   if(m_target->getObject() == nullptr)
    std::cout <<"whooooopsi1" <<std::endl;
    if(m_object == nullptr)
    std::cout <<"whooooopsi2" <<std::endl;

  if(m_target!=nullptr)
  {
 // std::cout <<"p1 " <<m_target->getHealth() <<std::endl;
    glm::vec3 d1 = m_target->getObject()->getPosition();
 //    std::cout <<"p1_" <<std::endl;
     d1 = m_object->getPosition();
    glm::vec3 dir = glm::normalize( m_target->getObject()->getPosition() - m_object->getPosition() );
    float distance = glm::distance(m_target->getObject()->getPosition() , m_object->getPosition());

    m_damage -= DAMAGE_DRAIN*delta;

    if(m_damage <=0.0f)
      return true;
    else
    {
      float s = ( MAX_SCALE / MAX_DAMAGE ) * m_damage;
      m_object->setScale(glm::vec3(s,s,s));
    }

    if( distance > 0.1f)
    {
      dir *= MOVEMENT_SPEED* delta;
      m_object->setPosition( m_object->getPosition() +dir );
    }
    else
    {
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
  std::cout<<"old target " <<old_target<< " current target"<<m_target <<std::endl;
  if( m_target == old_target)
  {
    Mob * nt = mobs->getClosestMobInRange(m_object->getPosition(), 1000.0f);
    m_target = nt;    
   std::cout<<"set target " <<m_target<<std::endl;
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