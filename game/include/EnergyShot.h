#ifndef ENERGYSHOT_H
#define ENERGYSHOT_H


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <vector>
#include <string>

class Scene;
class Object;
class Mob;
class Mobs;
class Environment;

// represents a plant 

class EnergyShot
{
public:
  EnergyShot( );
  ~EnergyShot();

  EnergyShot(const EnergyShot& other) = delete;
  EnergyShot& operator=(const EnergyShot& other) = delete;

  // inits the shot
  bool initialize(Scene * scene, Mob * target, glm::vec3 start_position, float damage);

  // target getter settter
  void changeTarget(Mob * old_target, Mobs * mobs);
  Mob * getTarget();

  // update function .... true if close to mob
  bool update(float delta, Mobs * mobs, bool * mob_killed, Environment * env);

  // returns the objetc
  Object * getObject();

private:

  // the object 
  Object * m_object;

  // its target
  Mob * m_target;

  // how much damage it does on impact
  float m_damage;

};

#endif // ENERGYSHOT_H