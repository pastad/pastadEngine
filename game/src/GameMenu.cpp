#include "GameMenu.h"

#include "Engine.h"
#include "Log.h"
#include "GUI.h"
#include "Button.h"
#include "Image.h"

#define SLIDE_SIDE_OFFSET 500.0f
#define SLIDE_TOP_OFFSET 300

#include <iostream>

GameMenu::GameMenu():m_launch_game(true),m_selected(0)
{
}
GameMenu::~GameMenu()
{  
}

bool GameMenu::initialize(float width, float height)
{
  m_gui = Engine::addGUI();

  Image * img = m_gui->addImage();
  img->load("game/models/menu_background.jpg");

  m_img_instructions = m_gui->addImage();
  m_img_instructions->load("game/models/graphics/Instructions.png");
  m_img_instructions->setInactive();

  m_img_credits = m_gui->addImage();
  m_img_credits->load("game/models/graphics/Credits.png");


  float ssize1 = width - (SLIDE_SIDE_OFFSET /2.0f);
  float ssize2 = height - SLIDE_TOP_OFFSET;

  float s1  = ssize1 / m_img_credits->getSize().x;
  float s2  = ssize2 / m_img_credits->getSize().y;

  m_img_credits->setInactive();
  m_img_credits->setScale(glm::vec2(s1,s2));
  m_img_credits->setPosition(glm::vec2(SLIDE_SIDE_OFFSET/2.0f,0));

  m_img_instructions->setScale(glm::vec2(s1,s2));
  m_img_instructions->setPosition(glm::vec2(SLIDE_SIDE_OFFSET/2.0f,0));


  m_btn_play =  m_gui->addButton();
  m_btn_play->intitializeWithToggle("game/models/menu_play_toggled.png","game/models/menu_play.png",
  glm::vec2(Engine::getWindowWidth()/2.0f-100.0f,Engine::getWindowHeight()/2.0f),  glm::vec2(0.3f,0.3f), "Play", true);

  m_btn_instructions =  m_gui->addButton();
  m_btn_instructions->intitializeWithToggle("game/models/menu_instructions_toggled.png","game/models/menu_instructions.png",
  glm::vec2(Engine::getWindowWidth()/2.0f-100.0f,Engine::getWindowHeight()/2.0f-50.0f),  glm::vec2(0.3f,0.3f), "Instructions", false);

  m_btn_credits =  m_gui->addButton();
  m_btn_credits->intitializeWithToggle("game/models/menu_credits_toggled.png","game/models/menu_credits.png",
  glm::vec2(Engine::getWindowWidth()/2.0f-100.0f,Engine::getWindowHeight()/2.0f-100.0f),  glm::vec2(0.3f,0.3f), "Credits", false);

  m_btn_exit =  m_gui->addButton();
  m_btn_exit->intitializeWithToggle("game/models/menu_exit_toggled.png","game/models/menu_exit.png",
  glm::vec2(Engine::getWindowWidth()/2.0f-100.0f,Engine::getWindowHeight()/2.0f-150.0f),  glm::vec2(0.3f,0.3f), "Exit", false);

  m_active = true;

  m_gui->registerButtonPressedCallback(&mouseButtonCallback);

 // m_background_sound_buffer = new sf::SoundBuffer();
 // m_background_sound_sound = new sf::Sound();

  if (!m_background_sound_buffer.loadFromFile("game/models/sounds/canary.wav"))
  {
    Engine::getLog()->log("GameMenu", "couldn't load file");
   // delete m_background_sound_sound;
   // m_background_sound_sound = nullptr;
   // delete m_background_sound_buffer;
   // m_background_sound_buffer = nullptr;
    return false;
  }

  m_background_sound_sound.setBuffer(m_background_sound_buffer);
  m_background_sound_sound.play();
  m_background_sound_sound.setLoop(true);

  m_selected =0;
  m_selected_slide = GM_SLIDE_MAIN;

  std::cout << "menu loaded" <<std::endl;

  return true;
}

bool GameMenu::unload()
{
  Engine::removeGUI(m_gui);

  m_background_sound_sound.stop();

  //if( m_background_sound_sound != nullptr)
  //  delete m_background_sound_sound;
 // if (m_background_sound_buffer != nullptr)
  //  delete m_background_sound_buffer;

  return true;
}

void GameMenu::update()
{
  if(m_active)
  {
    if( Engine::isKeyReleasedAndPressed(265))    
      m_selected--;
    
    if( Engine::isKeyReleasedAndPressed(264))    
      m_selected++;

    if(m_selected < 0)
      m_selected = 0;
    if(m_selected>3)
      m_selected =  3;

    refreshSelection();

    if( Engine::isKeyReleasedAndPressed(257))
    {
      if(m_selected_slide ==   GM_SLIDE_MAIN)
      {
        if(m_btn_play->isToggled())
        {
          m_launch_game = true;
          m_active = false;
        }
        if(m_btn_instructions->isToggled())
        {
          changeSlide(GM_SLIDE_INSTRUCTIONS);
        }
        if(m_btn_credits->isToggled())
        {
          changeSlide(GM_SLIDE_CREDITS);
        }
        if(m_btn_exit->isToggled())
        {
          m_launch_game = false;
          m_active = false;
        }
      }
      else
      {
        if(m_selected_slide ==  GM_SLIDE_INSTRUCTIONS)
        {
          changeSlide(GM_SLIDE_MAIN);
        }
        else
        {
          if(m_selected_slide ==  GM_SLIDE_CREDITS)
          {
            changeSlide(GM_SLIDE_MAIN);
          }
        }
      }
    }
  }
}

void GameMenu::refreshSelection()
{
  if(m_selected_slide == GM_SLIDE_MAIN)
  {
    if(m_selected == 0)
    {
      if(!m_btn_play->isToggled())
      {
        m_btn_play->togglOn();
        m_btn_instructions->togglOff();
        m_btn_credits->togglOff();
        m_btn_exit->togglOff();
      }
    }
    if(m_selected == 1)
    {
      if(!m_btn_instructions->isToggled())
      {
        m_btn_play->togglOff();
        m_btn_instructions->togglOn();
        m_btn_credits->togglOff();
        m_btn_exit->togglOff();
      }
    }
    if(m_selected == 2)
    {
      if(!m_btn_credits->isToggled())
      {
        m_btn_play->togglOff();
        m_btn_instructions->togglOff();
        m_btn_credits->togglOn();
        m_btn_exit->togglOff();
      }
    }
    if(m_selected == 3)
    {
      if(!m_btn_exit->isToggled())
      {
        m_btn_play->togglOff();
        m_btn_credits->togglOff();
        m_btn_instructions->togglOff();
        m_btn_exit->togglOn();
      }
    }
  }
}

void GameMenu::changeSlide(unsigned int slide)
{
  m_selected_slide = slide;

  if(slide == GM_SLIDE_MAIN)
  {
    m_btn_play->setActive();
    m_btn_credits->setActive();
    m_btn_instructions->setActive();
    m_btn_exit->setActive();

    m_img_instructions->setInactive();

    m_img_credits->setInactive();

    refreshSelection();
  }
  if(slide == GM_SLIDE_INSTRUCTIONS)
  {
    // std::cout << "Instructions" <<std::endl;
    m_btn_play->setInactive();
    m_btn_credits->setInactive();
    m_btn_instructions->setInactive();
    m_btn_exit->setInactive();

    m_img_instructions->setActive();

    m_img_credits->setInactive();
  }
  if(slide == GM_SLIDE_CREDITS)
  {
    m_btn_play->setInactive();
    m_btn_credits->setInactive();
    m_btn_instructions->setInactive();
    m_btn_exit->setInactive();

    m_img_instructions->setInactive();

    m_img_credits->setActive();
  }
}


bool GameMenu::isActive()
{
  return m_active;
}

bool GameMenu::shouldGameBeStarted()
{
  return m_launch_game;
}

void GameMenu::mouseButtonCallback(Button * btn)
{
  if(btn->getDescriptor() == "Play")
    std::cout <<"Play pressed"<<std::endl;
}
