#ifndef PLANT_H
#define PLANT_H


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <vector>
#include <string>

class Scene;
class Object;
class Mobs;
class Mob;
class Environment;


enum 
{
  PLANT_TREE,
  PLANT_ENERGY_FLOWER,
  PLANT_ATTACK_FLOWER,
  PLANT_TRAP_FLOWER
};

#define ENERGY_FLOWER_COST 25
#define ATTACK_FLOWER_COST 15
#define TRAP_FLOWER_COST 15


// represents a plant 

class Plant
{
public:
  Plant(Scene * scene , unsigned int type, glm::vec3 pos);
  ~Plant();

  Plant(const Plant& other) = delete;
  Plant& operator=(const Plant& other) = delete;

  // updates the plant
  void update(float delta, float sun_strength, Mobs * mobs, Environment * env);

  // returns the object
  Object * getObject();

  //returns the growth in percent
  float getGrowth();

  // returns the type of the plant
  unsigned int getType();

  // harvests the plant and returns the energy
  std::vector<Object *> harvest();

  // attacks mobs if conditions match
  void attackMobs(Mobs* mobs, Environment * env);

  // traps a mob
  void trapMobs(Mobs* mobs, Environment * env);

  // signals that target was eliminated
  void targeGone(Mob * m);

private:

  // the object 
  Object * m_object;

  // an additional object
  Object * m_additional_object = nullptr;
  Object * m_additional_object2 = nullptr;

  // how far the plant is grown (percantage)
  float m_growth;
  float m_energy_growth;

  // the cooldown time for the action
  float m_cooldown;

  //  hold time for trap plant  
  float m_hold_time;

  // the target for holding
  Mob * m_hold_target;

  // indicates how much energy is stored
  float m_stored_energy;

  // the type of the plant
  unsigned int m_type;

  // the energy the plant holds
  std::vector<Object *> m_energy;

  // the standard plant pos
  glm::vec3 m_standard_pos;

  // if trapplant extension of the grabber
  float m_extension_length = 0;

};

#endif // PLANT_H