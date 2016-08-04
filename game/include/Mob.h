#ifndef MOB_H
#define MOB_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Object;
class Scene;

class Player;

// represents a mobile in the game

class Mob
{
public:
  Mob();
  ~Mob();

  Mob(const Mob& other) = delete;
  Mob& operator=(const Mob& other) = delete;

  // initializes the mobile
  bool initialize(Scene * scene, float health);

  // updates the mobile
  void update(Player * player, float delta);

  // returns the object of the mob
  Object * getObject();

  // position setter
  void setPosition(glm::vec3 pos);

  // does damage to the mob | true if dead
  bool doDamage(float damage);

  // returns the health of the mob
  float getHealth();

private:

  // the objecy ... obviously
  Object  * m_object;

  // the health of the mob 0 == dead
  float m_health;

};

#endif