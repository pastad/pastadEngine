#ifndef GAME_H
#define GAME_H

#include <vector>

class Player;
class Scene;
class Mobs;
class Environment;

class Light;

// the main class of the game

class Game
{
public:
  Game();
  ~Game();

  Game(const Game& other) = delete;
  Game& operator=(const Game& other) = delete;


  // initializes the game
  bool initialize();

  // updates the game
  void update();

  // spawns the next wave
  void waveSpawn();

private:

  Player * m_player;

  Scene * m_scene;

  Mobs * m_mobs;

  Environment * m_environment;

  Light * m_sun;

  // day time 
  float m_game_time;

  // if the spawn this round was executed
  bool m_spawn_done;


};

#endif