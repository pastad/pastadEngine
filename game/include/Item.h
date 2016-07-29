#ifndef ITEM_H
#define ITEM_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Object;

// an item

class Item 
{
public:
  Item(Object * obj, bool is_weapon);
  ~Item();

  Item(const Item& other) = delete;
  Item& operator=(const Item& other) = delete;

  //sets the position and rotation
  void setPosition(glm::vec3 pos);
  void setRotation(glm::vec3 rot);

  // (in)active setters
  void setActive();
  void setInactive();

  // rteurns the object of the item
  Object * getObject();

  // isWeapon getter
  bool isWeapon();
private:

  // the object of the item
  Object * m_object;

  // true if it is a wapon
  bool m_is_weapon;

};

#endif // ITEM_H