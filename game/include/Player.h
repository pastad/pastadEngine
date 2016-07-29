#ifndef PLAYER_H
#define PLAYER_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Camera;
class Scene;
class Object;
class Inventory;
class GUI;

// the player character

class Player
{
public:
  Player();
  ~Player();

  Player(const Player& other) = delete;
  Player& operator=(const Player& other) = delete;


  // initializes the player
  static bool init( Scene * scene);

  // updates the player
  static void update();

  // returns the weapon object
  Object * getWeapon();

  // returns the position
  glm::vec3 getPosition();

private:

  // holds the camera pointer for the camera
  static Camera * m_camera;

  // the players inventory
  static Inventory * m_inventory;

  // the callback called by the camera
  static void cameraMovedCallback();

  //  the stats of the player
  static float m_health;
  static float m_hunger;

  // the player gui
  static GUI * m_gui;
};

#endif