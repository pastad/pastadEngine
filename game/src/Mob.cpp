#include "Mob.h"

#include "Player.h"

#include "Object.h"
#include "Scene.h"

#define MOVEMENT_SPEED 2.0f

Mob::Mob()
{  
}

Mob::~Mob()
{  
}

bool Mob::initialize(Scene * scene)
{
  m_object = scene->addObject("game/models/dark_energy_monster.obj",glm::vec3(-4,0.1,5), false);
  //m_object->setRotation(glm::vec3(0.0f,90.0f,90.0f));
  m_object->setScale(glm::vec3(0.25f,0.25f,0.25f));
  //m_object->applyPhysics();
  return true;
}

void Mob::update(Player * player, float delta)
{
  glm::vec3 d =glm::normalize( player->getPosition()-glm::vec3(0,0.8f,0) - m_object->getPosition() );
  glm::vec3 step = d * delta * MOVEMENT_SPEED;
  setPosition( m_object->getPosition()+ step );
}

Object * Mob::getObject()
{
  return m_object;
}

void Mob::setPosition(glm::vec3 pos)
{
	m_object->setPosition(pos);
}