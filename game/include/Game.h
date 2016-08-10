#ifndef GAME_H
#define GAME_H

#include <vector>

#include <SFML/Audio.hpp>

#define SOUND_EFFECT_VOLUME 30
#define SOUND_BACKGROUND_VOLUME 10


class Player;
class Scene;
class Mobs;
class Environment;
class SoundManager;

class Light;
class Skybox;

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

  // returns true if game has ended
  bool hasEnded();

  // ends the game
  static void end();

private:

  Player * m_player;

  Scene * m_scene;

  Mobs * m_mobs;

  Environment * m_environment;

  Light * m_sun;

  Skybox * m_skybox;

  // the background sound
  sf::SoundBuffer * m_background_sound_buffer;
  sf::Sound * m_background_sound_sound;

  // day time 
  float m_game_time;

  // and the time after death;
  static float m_end_time;

  // if the spawn this round was executed
  bool m_spawn_done;

  // true if game should end
  bool m_ended;



};

#endif