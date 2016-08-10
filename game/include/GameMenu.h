#ifndef GAME_MENU_H
#define GAME_MENU_H


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include <SFML/Audio.hpp>

enum
{
  GM_SLIDE_MAIN,
  GM_SLIDE_INSTRUCTIONS,
  GM_SLIDE_CREDITS
};


class GUI;
class Button;
class Image;

// represents the menu of the game 

class GameMenu
{
public:
  GameMenu( );
  ~GameMenu();

  GameMenu(const GameMenu& other) = delete;
  GameMenu& operator=(const GameMenu& other) = delete;

  // initializes the menu
  bool initialize(float width, float height);

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
  Button *m_btn_instructions;
  Button *m_btn_back;

  // instructions and credits
  Image * m_img_instructions;
  Image * m_img_credits;

  // the selected button
  unsigned int m_selected;

  // the current slide(main/credit/instructions) 
  unsigned int m_selected_slide;

  // background sound
  sf::SoundBuffer * m_background_sound_buffer;
  sf::Sound * m_background_sound_sound;

  // refresh and slide change
  void refreshSelection();
  void changeSlide(unsigned int slide);

  

};

#endif // GAME_MENU_H