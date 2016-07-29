#include "Item.h"

#include "Object.h"

#include <iostream>

Item::Item(Object * obj, bool is_weapon): m_is_weapon(is_weapon)
{  
  m_object = obj;
}
Item::~Item()
{  
}

void Item::setPosition(glm::vec3 pos)
{
  m_object->setPosition(pos);
}

void Item::setRotation(glm::vec3 rot)
{
  m_object->setRotation(rot);
}

void Item::setActive()
{
  m_object->setVisible();
}

void Item::setInactive()
{
  m_object->setInvisible();
}

Object * Item::getObject()
{
  return m_object;
}
bool Item::isWeapon()
{
  return m_is_weapon;
}