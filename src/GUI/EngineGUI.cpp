#include "EngineGUI.h"

#include "Button.h"
#include "Image.h"
#include "Text.h"

#include "Engine.h"
#include "Scene.h"
#include "Camera.h"
#include "Log.h"

Image * EngineGUI::m_back_panel;
Image * EngineGUI::m_back_panel_edit;

Button * EngineGUI::m_tb_camera;
Text * EngineGUI::m_txt_camera;

Button * EngineGUI::m_tb_fxaa;
Text * EngineGUI::m_txt_fxaa;

Button * EngineGUI::m_tb_hdr;
Text * EngineGUI::m_txt_hdr;

Button * EngineGUI::m_tb_bloom;
Text * EngineGUI::m_txt_bloom;

Button * EngineGUI::m_tb_shadow_pcf;
Text * EngineGUI::m_txt_shadow_pcf;

Button * EngineGUI::m_tb_shadow_standard;
Text * EngineGUI::m_txt_shadow_standard;

Button * EngineGUI::m_tb_shadow_rs;
Text * EngineGUI::m_txt_shadow_rs;

Button * EngineGUI::m_btn_refresh_shader;
Text * EngineGUI::m_txt_refresh_shader;



EngineGUI::EngineGUI() :GUI( 0 )
{  
}

EngineGUI::~EngineGUI()
{  
}


// initialization -------------------------------------------------

bool EngineGUI::initialize()
{
  Button * bt = GUI::getButton();
 
  bt->intitialize("resources/expand.png","",glm::vec2(0.0f,Engine::getWindowHeight()-40.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "expand");
 

  m_btn_refresh_shader = GUI::getButton();
  m_btn_refresh_shader->intitialize("resources/btn.png","",glm::vec2(20.0f,Engine::getWindowHeight()-70.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "refresh_shaders");
  m_btn_refresh_shader->setInactive();

  m_txt_refresh_shader = GUI::getText();
  m_txt_refresh_shader->setPosition(glm::vec2(40.0f,Engine::getWindowHeight()-70.0f));
  m_txt_refresh_shader->setScale(0.2f);
  m_txt_refresh_shader->setColor(glm::vec3(0,0,0));
  m_txt_refresh_shader->setText("Reload Shaders");
  m_txt_refresh_shader->setInactive();

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

  m_tb_fxaa = GUI::getButton();
  m_tb_fxaa->intitializeWithToggle("resources/toggle_on.png","resources/toggle_off.png",
    glm::vec2(20.0f,Engine::getWindowHeight()-400.0f),glm::vec2(0.5f,0.5f),
    "fxaa", true);
  m_tb_fxaa->setInactive();

  m_txt_fxaa = GUI::getText();
  m_txt_fxaa->setPosition(glm::vec2(40.0f,Engine::getWindowHeight()-400.0f));
  m_txt_fxaa->setScale(0.2f);
  m_txt_fxaa->setColor(glm::vec3(0,0,0));
  m_txt_fxaa->setText("FXAA");
  m_txt_fxaa->setInactive();

  m_tb_hdr = GUI::getButton();
  m_tb_hdr->intitializeWithToggle("resources/toggle_on.png","resources/toggle_off.png",
    glm::vec2(20.0f,Engine::getWindowHeight()-420.0f),glm::vec2(0.5f,0.5f),
    "hdr", true);
  m_tb_hdr->setInactive();

  m_txt_hdr = GUI::getText();
  m_txt_hdr->setPosition(glm::vec2(40.0f,Engine::getWindowHeight()-420.0f));
  m_txt_hdr->setScale(0.2f);
  m_txt_hdr->setColor(glm::vec3(0,0,0));
  m_txt_hdr->setText("HDR");
  m_txt_hdr->setInactive();

  m_tb_bloom = GUI::getButton();
  m_tb_bloom->intitializeWithToggle("resources/toggle_on.png","resources/toggle_off.png",
    glm::vec2(20.0f,Engine::getWindowHeight()-440.0f),glm::vec2(0.5f,0.5f),
    "bloom", true);
  m_tb_bloom->setInactive();

  m_txt_bloom = GUI::getText();
  m_txt_bloom->setPosition(glm::vec2(40.0f,Engine::getWindowHeight()-440.0f));
  m_txt_bloom->setScale(0.2f);
  m_txt_bloom->setColor(glm::vec3(0,0,0));
  m_txt_bloom->setText("Bloom");
  m_txt_bloom->setInactive();


  m_tb_shadow_pcf = GUI::getButton();
  m_tb_shadow_pcf->intitializeWithToggle("resources/toggle_on.png","resources/toggle_off.png",
    glm::vec2(30.0f,Engine::getWindowHeight()-320.0f),glm::vec2(0.5f,0.5f),
    "shadowPCF", true);
  m_tb_shadow_pcf->setInactive();

  m_txt_shadow_pcf = GUI::getText();
  m_txt_shadow_pcf->setPosition(glm::vec2(50.0f,Engine::getWindowHeight()-320.0f));
  m_txt_shadow_pcf->setScale(0.2f);
  m_txt_shadow_pcf->setColor(glm::vec3(0,0,0));
  m_txt_shadow_pcf->setText("Percentag Closer Filtering");
  m_txt_shadow_pcf->setInactive();

  m_tb_shadow_rs = GUI::getButton();
  m_tb_shadow_rs->intitializeWithToggle("resources/toggle_on.png","resources/toggle_off.png",
    glm::vec2(30.0f,Engine::getWindowHeight()-340.0f),glm::vec2(0.5f,0.5f),
    "shadowRS", false);
  m_tb_shadow_rs->setInactive();

  m_txt_shadow_rs = GUI::getText();
  m_txt_shadow_rs->setPosition(glm::vec2(50.0f,Engine::getWindowHeight()-340.0f));
  m_txt_shadow_rs->setScale(0.2f);
  m_txt_shadow_rs->setColor(glm::vec3(0,0,0));
  m_txt_shadow_rs->setText("Random Sampling");
  m_txt_shadow_rs->setInactive();

  m_tb_shadow_standard = GUI::getButton();
  m_tb_shadow_standard->intitializeWithToggle("resources/toggle_on.png","resources/toggle_off.png",
    glm::vec2(20.0f,Engine::getWindowHeight()-300.0f),glm::vec2(0.5f,0.5f),
    "shadowStandard", true);
  m_tb_shadow_standard->setInactive();

  m_txt_shadow_standard = GUI::getText();
  m_txt_shadow_standard->setPosition(glm::vec2(40.0f,Engine::getWindowHeight()-300.0f));
  m_txt_shadow_standard->setScale(0.2f);
  m_txt_shadow_standard->setColor(glm::vec3(0,0,0));
  m_txt_shadow_standard->setText("Shadows");
  m_txt_shadow_standard->setInactive();


  GUI::registerButtonPressedCallback(mouseButtonCallback);
  Engine::getLog()->log("EngineGUI", "initialized");
  return true;
}


// callbacks -------------------------------------------------

void EngineGUI::mouseButtonCallback(Button * btn)
{
  //Engine::getLog()->log("EngineUI",btn->getDescriptor());
  if(btn->getDescriptor() == "expand")
  {
    Engine::getLog()->log("EngineGUI", "expanding ui");
    btn->setImage("resources/collapse.png");
    btn->setDescriptor("collapse");
    btn->setPositionAndScale(glm::vec2(200.0f,Engine::getWindowHeight()-40.0f),glm::vec2(0.5f,0.5f));
    m_back_panel->setActive();
    m_tb_camera->setActive();
    m_txt_camera->setActive();
    m_tb_fxaa->setActive();
    m_txt_fxaa->setActive();
    m_tb_hdr->setActive();
    m_txt_hdr->setActive();
    m_tb_bloom->setActive();
    m_txt_bloom->setActive();
    m_tb_shadow_pcf->setActive();
    m_txt_shadow_pcf->setActive();
    m_tb_shadow_standard->setActive();
    m_txt_shadow_standard->setActive();
    m_tb_shadow_rs->setActive();
    m_txt_shadow_rs->setActive();
    m_btn_refresh_shader->setActive();
    m_txt_refresh_shader->setActive();
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
      m_tb_fxaa->setInactive();
      m_txt_hdr->setInactive();
      m_tb_hdr->setInactive();
      m_tb_bloom->setInactive();
      m_txt_bloom->setInactive();
      m_txt_fxaa->setInactive();
      m_tb_shadow_pcf->setInactive();
      m_txt_shadow_pcf->setInactive();
      m_tb_shadow_standard->setInactive();
      m_txt_shadow_standard->setInactive();
      m_tb_shadow_rs->setInactive();
      m_txt_shadow_rs->setInactive();
      m_btn_refresh_shader->setInactive();
      m_txt_refresh_shader->setInactive();
    }
  }
 

  if(btn->getDescriptor() == "refresh_shaders")
  {
    Engine::refreshShaders();
  }
  if(btn->getDescriptor() == "camera_toggle")
  {
    Engine::getLog()->log("EngineUI","camera");
    if(btn->isToggled())
    {
      Engine::getScene()->getCamera()->setRotationAllowed();
    }
    else
    {
      Engine::getScene()->getCamera()->setRotationNotAllowed();
    }
  }
  if(btn->getDescriptor() == "fxaa")
  {
    if(btn->isToggled())
    {
      Engine::setPostProcessing(PP_FXAA,true);
    }
    else
    {
      Engine::setPostProcessing(PP_FXAA,false);
    }
  }
  if(btn->getDescriptor() == "hdr")
  {
    if(btn->isToggled())
    {
      Engine::setPostProcessing(PP_HDR,true);
    }
    else
    {
      Engine::setPostProcessing(PP_HDR,false);
    }
  }
  if(btn->getDescriptor() == "bloom")
  {
    if(btn->isToggled())
    {
      Engine::setPostProcessing(PP_BLOOM,true);
    }
    else
    {
      Engine::setPostProcessing(PP_BLOOM,false);
    }
  }

  if(btn->getDescriptor() == "shadowPCF")
  {
    if(btn->isToggled())
    {
      m_tb_shadow_standard->togglOn();
      Engine::setShadowTechnique(ST_STANDARD_PCF);
      if(m_tb_shadow_rs->isToggled())
      {
        m_tb_shadow_rs->togglOff();
      }
    }
    else
    {
      if( m_tb_shadow_standard->isToggled())
      {
        Engine::setShadowTechnique(ST_STANDARD);
      }
    }
  }
   if(btn->getDescriptor() == "shadowRS")
  {
    if(btn->isToggled())
    {
      m_tb_shadow_standard->togglOn();
      Engine::setShadowTechnique(ST_STANDARD_RS);
      if(m_tb_shadow_pcf->isToggled())
      {
        m_tb_shadow_pcf->togglOff();
      }
    }
    else
    {
      if( m_tb_shadow_standard->isToggled())
      {
        Engine::setShadowTechnique(ST_STANDARD);
      }
    }

  }
  if(btn->getDescriptor() == "shadowStandard")
  {
    if(btn->isToggled())
    {
      bool set = false;

      if( m_tb_shadow_pcf->isToggled())
      {
        Engine::setShadowTechnique(ST_STANDARD_PCF);
        set = true;
      }
      if( m_tb_shadow_pcf->isToggled())
      {
        Engine::setShadowTechnique(ST_STANDARD_RS);
        set = true;
      }
      if(!set)
        Engine::setShadowTechnique(ST_STANDARD);
    }
    else
    {
      Engine::setShadowTechnique(ST_NONE);
      if( m_tb_shadow_pcf->isToggled())
        m_tb_shadow_pcf->togglOff();
      if( m_tb_shadow_rs->isToggled())
        m_tb_shadow_rs->togglOff();
    }
  }
 
}

