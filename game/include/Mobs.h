#ifndef MOBS_H
#define MOBS_H

#include <vector>

class Mob;
class Scene;
class Player;

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
  void update(Player * player);


private:

  // the mob list
  std::vector<Mob *> m_mobs;

};

#endif