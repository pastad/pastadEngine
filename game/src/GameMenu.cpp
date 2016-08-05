#include "GameMenu.h"

#include "Engine.h"
#include "GUI.h"
#include "Button.h"
#include "Image.h"

#include <iostream>

GameMenu::GameMenu():m_launch_game(false),m_selected(0)
{
}
GameMenu::~GameMenu()
{  
}

bool GameMenu::initialize()
{
  m_gui = Engine::addGUI();

  Image * img = m_gui->addImage();
  img->load("game/models/menu_background.jpg");

  m_btn_play =  m_gui->addButton();
  m_btn_play->intitializeWithToggle("game/models/menu_play_toggled.png","game/models/menu_play.png",
  glm::vec2(Engine::getWindowWidth()/2.0f-100.0f,Engine::getWindowHeight()/2.0f),  glm::vec2(0.5f,0.5f), "Play", true);

  m_btn_credits =  m_gui->addButton();
  m_btn_credits->intitializeWithToggle("game/models/menu_credits_toggled.png","game/models/menu_credits.png",
  glm::vec2(Engine::getWindowWidth()/2.0f-100.0f,Engine::getWindowHeight()/2.0f-50.0f),  glm::vec2(0.5f,0.5f), "Credits", false);

  m_btn_exit =  m_gui->addButton();
  m_btn_exit->intitializeWithToggle("game/models/menu_exit_toggled.png","game/models/menu_exit.png",
  glm::vec2(Engine::getWindowWidth()/2.0f-100.0f,Engine::getWindowHeight()/2.0f-100.0f),  glm::vec2(0.5f,0.5f), "Exit", false);

  m_active = true;

  m_gui->registerButtonPressedCallback(&mouseButtonCallback);

  return true;
}

bool GameMenu::unload()
{
  Engine::removeGUI(m_gui);
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
    if(m_selected>2)
      m_selected =  2;

    if(m_selected == 0)
    {
      if(!m_btn_play->isToggled())
      {
        m_btn_play->togglOn();
        m_btn_credits->togglOff();
        m_btn_exit->togglOff();
      }
    }
    if(m_selected == 1)
    {
      if(!m_btn_credits->isToggled())
      {
        m_btn_play->togglOff();
        m_btn_credits->togglOn();
        m_btn_exit->togglOff();
      }
    }
    if(m_selected == 2)
    {
      if(!m_btn_exit->isToggled())
      {
        m_btn_play->togglOff();
        m_btn_credits->togglOff();
        m_btn_exit->togglOn();
      }
    }

    if( Engine::isKeyReleasedAndPressed(257))
    {
      if(m_btn_play->isToggled())
      {
        m_launch_game = true;
        m_active = false;
      }
      if(m_btn_exit->isToggled())
      {
        m_launch_game = false;
        m_active = false;
      }
    }
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
