#ifndef MOB_H
#define MOB_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Object;
class Scene;

// represents a mobile in the game

class Mob
{
public:
  Mob();
  ~Mob();

  Mob(const Mob& other) = delete;
  Mob& operator=(const Mob& other) = delete;

  // initializes the mobile
  bool initialize(Scene * scene);

  // updates the mobile
  void update();

  // returns the object of the mob
  Object * getObject();

  // position setter
  void setPosition(glm::vec3 pos);

private:

  Object  * m_object;

};

#endif