#ifndef GAME_H
#define GAME_H

#include <vector>

#include <SFML/Audio.hpp>


#define DAY_LENGTH 90
#define DEG_PER_SEC 360.0f / DAY_LENGTH

#define SHADOW_TIME 2

class Player;
class Scene;
class Mobs;
class Environment;
class SoundManager;

class Light;
class Skybox;

struct HighScore
{
  unsigned int days;
  float time;
  float energy_left;
};


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

  // returns the game time
  static float getGameTime();

  // config getters
  static unsigned int getSoundEffectVolume();
  static unsigned int getSoundBackgroundVolume();

  // saving and loading the game config
  bool saveConfig(std::string path, unsigned int effect_volume, unsigned int background_volume);
  bool loadConfig(std::string path);
  static  bool readConfig(std::string path, unsigned int * sev, unsigned int * sbv);

  //saving and loading the highscore
  static bool readHighsore(std::string path, HighScore *last, HighScore * best);


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

  // day time day
  static float m_game_time;
  unsigned int m_game_day;

  // and the time after death;
  static float m_end_time;

  // if the spawn this round was executed
  bool m_spawn_done;

  // true if game should end
  bool m_ended;

  // the highscores
  HighScore m_last_highscore;
  HighScore m_best_highscore;

  // the configs
  static unsigned int m_sound_effect_volume ;
  static unsigned int m_sound_background_volume ;

  void loadHighscore();
  void saveHighscore();

};

#endif