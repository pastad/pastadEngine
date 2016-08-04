#ifndef PLANT_H
#define PLANT_H


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <vector>
#include <string>

class Scene;
class Object;
class Mobs;
class Environment;


enum 
{
  PLANT_TREE,
  PLANT_ENERGY_FLOWER,
  PLANT_ATTACK_FLOWER
};

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

private:

  // the object 
  Object * m_object;

  // how far the plant is grown (percantage)
  float m_growth;
  float m_energy_growth;

  // the cooldown for the action
  float m_cooldown;

  // indicates how much energy is stored
  float m_stored_energy;

  // the type of the plant
  unsigned int m_type;

  // the energy the plant holds
  std::vector<Object *> m_energy;

};

#endif // PLANT_H