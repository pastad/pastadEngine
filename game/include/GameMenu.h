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
  GM_SLIDE_CREDITS,
  GM_SLIDE_OPTIONS
};

enum
{
  GM_STATE_FADE_IN,
  GM_STATE_ACTIVE,
  GM_STATE_FADE_OUT
};

enum
{
  SPEC_SIZE_1920_1080,
  SPEC_SIZE_1680_1050,
  SPEC_SIZE_1270_720
};

#define FADE_IN_TIME 2.0f


class GUI;
class Button;
class Image;
class Text;

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
  Button *m_btn_options;
  Button *m_btn_credits;
  Button *m_btn_exit;
  Button *m_btn_instructions;
  Button *m_btn_back;  

  Text * m_txt_options_size;
  Text * m_txt_options_fullscreen;
  Text * m_txt_options_shadows;
  Text * m_txt_options_sound_effect;
  Text * m_txt_options_sound_background;
  Text * m_txt_options_notice;

  Text * m_txt_highscore_last;
  Text * m_txt_highscore_best;


  // instructions and credits
  Image * m_img_instructions;
  Image * m_img_credits;

  // the background
  Image * m_menu_background;

  // the selected button
  unsigned int m_selected;

  // the current slide(main/credit/instructions) 
  unsigned int m_selected_slide;

  unsigned int m_main_state; 
  float m_main_state_timer;

  // option specs
  unsigned int m_spec_size;
  bool m_spec_fullscreen ;
  unsigned int m_spec_shadow ;
  unsigned int m_spec_sound_effect_volume;
  unsigned int m_spec_sound_background_volume;


  // background sound
  sf::SoundBuffer  m_background_sound_buffer;
  sf::Sound  m_background_sound_sound;

  // refresh and slide change
  void refreshSelection();
  void changeSlide(unsigned int slide);
  void deactivateOptions();
  void activateOptions();



  

};

#endif // GAME_MENU_H