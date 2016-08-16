#ifndef ENVIRONEMT_H
#define ENVIRONEMT_H


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <vector>
#include <string>

class Scene;
class Object;

class Plant;
class Player;
class Mobs;
class EnergyShot;
class EnergySpark;
class Mob;

// combines all environment objects 

class Environment
{
public:
  Environment();
  ~Environment();

  Environment(const Environment& other) = delete;
  Environment& operator=(const Environment& other) = delete;

  // inits the environment
  bool initialize(Scene * scene);

  // adds a plant to the environment
  void addPlant(Scene * scene , unsigned int type, glm::vec3 pos);

  // adds a shot to the environment
  void addEnergyShot(Scene * scene, Mob * target, glm::vec3 start_position  );

  // adds an energy spark to the environemnt
  void addEnergySpark(glm::vec3 start, glm::vec3 target, unsigned int type);
  void addEnergySpark( Object * object, glm::vec3 target, unsigned int type );

  // adds energy remains to the environment
  void addEnergyRemains(std::vector<Object *> objs);

  // updates the plant
  void update(float delta, float sun_strength, Player * player, Mobs * mobs);

  // checks what should be done if an action is triggered
  void checkAction(Player * player);

  // returns the plant which the obj belongs to
  Plant * getPlant(Object * obj);

  // refreshes targets with old as target
  void refreshTargets(Mob * old, Mobs * mobs);

private:

  // the different kind of plants
  std::vector<Plant*> m_plants;

  // active remains
  std::vector<Object*> m_energy_remains;

  // active shots
  std::vector<EnergyShot*> m_energy_shots;

  // the active sparks
  std::vector<EnergySpark*> m_energy_sparks;

};

#endif // ENVIRONEMT_H