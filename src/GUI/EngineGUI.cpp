#include "EngineGUI.h"

#include "Button.h"
#include "Image.h"
#include "Text.h"

#include "Engine.h"
#include "Scene.h"
#include "Camera.h"
#include "Log.h"

Image * EngineGUI::m_back_panel;
Button * EngineGUI::m_tb_camera;
Text * EngineGUI::m_txt_camera;

EngineGUI::EngineGUI() :GUI( 0 )
{  
}
EngineGUI::~EngineGUI()
{  
}

bool EngineGUI::initialize()
{
  Button * bt = GUI::getButton();
 
  bt->intitialize("resources/expand.png","",glm::vec2(0.0f,Engine::getWindowHeight()-40.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "expand");

  m_back_panel = GUI::getImage();
  m_back_panel->load("resources/panel.png");
  m_back_panel->setPosition(glm::vec2(0,Engine::getWindowHeight() -540.0f));
  m_back_panel->setInactive();


  m_tb_camera = GUI::getButton();
  m_tb_camera->intitializeWithToggle("resources/toggle_on.png","resources/toggle_off.png",
    glm::vec2(20.0f,Engine::getWindowHeight()-40.0f),glm::vec2(0.5f,0.5f),
    "camera_toggle", false);
  m_tb_camera->setInactive();

  m_txt_camera = GUI::getText();
  m_txt_camera->setPosition(glm::vec2(40.0f,Engine::getWindowHeight()-38.0f));
  m_txt_camera->setScale(0.2f);
  m_txt_camera->setColor(glm::vec3(0,0,0));
  m_txt_camera->setText("Camera Rotation");
  m_txt_camera->setInactive();


  GUI::registerButtonPressedCallback(mouseButtonCallback);
  Engine::getLog()->log("EngineGUI", "initialized");
  return true;
}

void EngineGUI::mouseButtonCallback(Button * btn)
{
  if(btn->getDescriptor() == "expand")
  {
    Engine::getLog()->log("EngineGUI", "expanding ui");
    btn->setImage("resources/collapse.png");
    btn->setDescriptor("collapse");
    btn->setPositionAndScale(glm::vec2(200.0f,Engine::getWindowHeight()-40.0f),glm::vec2(0.5f,0.5f));
    m_back_panel->setActive();
    m_tb_camera->setActive();
    m_txt_camera->setActive();
  }
  else
  {
    if(btn->getDescriptor() == "collapse")
    {
      Engine::getLog()->log("EngineGUI", "collapsing ui");
      btn->setImage("resources/expand.png");
      btn->setPositionAndScale(glm::vec2(0.0f,Engine::getWindowHeight()-40.0f),glm::vec2(0.5f,0.5f));
      btn->setDescriptor("expand");
      m_back_panel->setInactive();
      m_tb_camera->setInactive();
      m_txt_camera->setInactive();
    }
  }
  if(btn->getDescriptor() == "camera_toggle")
  {
    if(btn->isToggled())
    {
      Engine::getScene()->getCamera()->setRotationAllowed();
    }
    else
    {
      Engine::getScene()->getCamera()->setRotationNotAllowed();
    }
  }
}