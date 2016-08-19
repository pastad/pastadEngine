#include "Mob.h"

#include "Player.h"

#include "Object.h"
#include "Scene.h"

#include "Script.h"
#include "RotationScriptElement.h"

#define MOVEMENT_SPEED 2.0f
#define MAX_HEALTH 50.0f
#define MAX_SCALE 0.2f

#include <iostream>

Mob::Mob()
{  
  m_object = nullptr;
}

Mob::~Mob()
{  
  m_object = nullptr;
}

bool Mob::initialize(Scene * scene, float health)
{
  m_health = health;
  m_object = scene->addObject("game/models/dark_energy_monster.obj",glm::vec3(-4,0.1,5), false);
  if(m_object == nullptr)
    return false;
  //m_object->setRotation(glm::vec3(0.0f,90.0f,90.0f));
  m_object->setScale(glm::vec3(MAX_SCALE,MAX_SCALE,MAX_SCALE));
  Script * script = m_object->addScript();
  RotationScriptElement * rse = script->addRotationScript();
  rse->setupAlways();
  rse->setup(glm::vec3(0,1000.0f,0));

  float s =( MAX_SCALE / MAX_HEALTH ) * m_health;
  m_object->setScale(glm::vec3(s,s,s));

  m_moveable = true;

  return true;
}

void Mob::update(Player * player, float delta)
{
  if(isMoveable())
  {
    glm::vec3 d =glm::normalize( player->getPosition()-glm::vec3(0,0.8f,0) - m_object->getPosition() );
    glm::vec3 step = d * delta * MOVEMENT_SPEED;
    setPosition( m_object->getPosition()+ step );
  }
}

Object * Mob::getObject()
{
  return m_object;
}

void Mob::setPosition(glm::vec3 pos)
{
	m_object->setPosition(pos);
}

bool Mob::doDamage(float damage)
{
  m_health-= damage;
  if(m_health <=0.0f)
  {
    return true;
  }
  else
  {
    float s =( MAX_SCALE / MAX_HEALTH ) * m_health;
    m_object->setScale(glm::vec3(s,s,s));
    return false;
  }
}
float Mob::getHealth()
{
  return m_health;
}

void Mob::setMoveable()
{
  m_moveable = true;
}
void Mob::setNotMoveable()
{
  m_moveable = false;
}
bool Mob::isMoveable()
{
  return m_moveable;
}