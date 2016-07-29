#include "Mob.h"

#include "Object.h"
#include "Scene.h"

Mob::Mob()
{  
}

Mob::~Mob()
{  
}

bool Mob::initialize(Scene * scene)
{
  m_object = scene->addObject("game/models/bug.obj",glm::vec3(-4,0.1,5), false);
  //m_object->setRotation(glm::vec3(0.0f,90.0f,90.0f));
  m_object->setScale(glm::vec3(0.25f,0.25f,0.25f));
  m_object->applyPhysics();
  return true;
}

void Mob::update()
{

}

Object * Mob::getObject()
{
  return m_object;
}

void Mob::setPosition(glm::vec3 pos)
{
	m_object->setPosition(pos);
}