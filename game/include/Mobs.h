#ifndef MOBS_H
#define MOBS_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <vector>

class Mob;
class Scene;
class Player;
class Environment;

// represents all mobs 

class Mobs
{
public:
  Mobs();
  ~Mobs();

  Mobs(const Mobs& other) = delete;
  Mobs& operator=(const Mobs& other) = delete;


  // initializes the mobile
  bool initialize(Scene * scene);

  // updates the mobile
  void update(Player * player, Environment * env);

  // spawns random mobs
  void spawnRandom(Scene * scene, unsigned int amount);

  // removes a mob
  void removeMob(Mob * m);

  // returns the closest mob in range
  Mob * getClosestMobInRange(glm::vec3 pos , float range);

  // returns all mobs
  std::vector<Mob *> getMobs();

  // spawns the next wave
  void spawnNextWave();


private:

  // the mob list
  std::vector<Mob *> m_mobs;

  // the mob amount
  std::vector<int> m_waves;

};

#endif