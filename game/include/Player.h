#ifndef PLAYER_H
#define PLAYER_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Camera;
class Scene;
class Object;
class Inventory;

class GUI;
class Text;
class Environment;

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
  static Object * getWeapon();

  // returns the position
  static glm::vec3 getPosition();

  // adds/ removes energy to the player
  static void gainEngery(float val);
  static void drainEnergy(float val);

  // returns the energy stored by the player
  static float getEnergy();

  // checks the players actions
  static void checkAction(Environment * environment);
  static void checkSecondaryAction(Environment * environment);

private:

  // holds the camera pointer for the camera
  static Camera * m_camera;

  // the players inventory
  static Inventory * m_inventory;

  //  the stats of the player
  static float m_energy;

  // the player gui
  static GUI * m_gui;
  static Text * m_lower_text;

  // the callback called by the camera
  static void cameraMovedCallback();

  // sets up the gui
  static void setupGUI();
};

#endif