#ifndef GAME_MENU_H
#define GAME_MENU_H


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <vector>
#include <string>


class GUI;
class Button;

// represents the menu of the game 

class GameMenu
{
public:
  GameMenu( );
  ~GameMenu();

  GameMenu(const GameMenu& other) = delete;
  GameMenu& operator=(const GameMenu& other) = delete;

  // initializes the menu
  bool initialize();

  // unloads/cleans-up the menu
  bool unload();

  // updates the menu
  void update();

  // true if menu is still active
  bool isActive();

  // true if game should be launched
  bool shouldGameBeStarted();

private:

  GUI * m_gui;

  // true if menu is active
  bool m_active;

  // true if game should be launched
  bool m_launch_game;

  static void mouseButtonCallback(Button * btn);

  // the buttons
  Button *m_btn_play;
  Button *m_btn_credits;
  Button *m_btn_exit;

  // the selected button
  unsigned int m_selected;


  

};

#endif // GAME_MENU_H