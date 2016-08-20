#ifndef ITEM_H
#define ITEM_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>


enum
{
  ITEM_PLANT_TOOL,
  ITEM_HARVEST_TOOL,
  ITEM_ATTACK_PLANT_TOOL,
  ITEM_TRAP_PLANT_TOOL
};

class Object;
class Environment;
class Player;

// an item

class Item 
{
public:
  Item(Object * obj, bool is_weapon, unsigned int type);
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

  // returns the item type
  unsigned int getType();

  // isWeapon getter
  bool isWeapon();

  // execute the action
  void doAction(Environment * environment);
  void doSecondaryAction(Environment * environment);

  // the needed energy for planting
  float getNeededEnergy();
  
private:

  // the object of the item
  Object * m_object;

  // true if it is a wapon
  bool m_is_weapon;

  // the type of the item
  unsigned int m_type;

  // checks where to place the item
  bool getPlacement( glm::vec3 *pos);
};

#endif // ITEM_H