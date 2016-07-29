#ifndef GAME_H
#define GAME_H

#include <vector>

class Player;
class Scene;
class Mobs;

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


private:

  Player * m_player;

  Scene * m_scene;

  Mobs * m_mobs;

};

#endif