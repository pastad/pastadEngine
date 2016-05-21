#include "SceneEditor.h"

#include "IOSubsystem.h"
#include "Button.h"
#include "Image.h"
#include "Text.h"

#include "Engine.h"
#include "Scene.h"
#include "Camera.h"
#include "Log.h"

#include "Light.h"
#include "Object.h"

#include <iostream>
#include <sstream>
#include <iomanip>  

Image * SceneEditor::m_back_panel_edit;

Button * SceneEditor::m_btn_del_light;
Text * SceneEditor::m_txt_del_light;

Button * SceneEditor::m_btn_add_light;
Text * SceneEditor::m_txt_add_light;

Button * SceneEditor::m_btn_add_spot_light;
Text * SceneEditor::m_txt_add_spot_light;

Button * SceneEditor::m_btn_add_object;
Text * SceneEditor::m_txt_add_object;

Button * SceneEditor::m_btn_x_plus;
Button * SceneEditor::m_btn_x_minus;
Button * SceneEditor::m_btn_y_plus;
Button * SceneEditor::m_btn_y_minus;
Button * SceneEditor::m_btn_z_plus;
Button * SceneEditor::m_btn_z_minus;

Button * SceneEditor::m_btn_rot1_plus;
Button * SceneEditor::m_btn_rot1_minus;
Button * SceneEditor::m_btn_rot2_plus;
Button * SceneEditor::m_btn_rot2_minus;

Text * SceneEditor::m_txt_xz_coords;
Text * SceneEditor::m_txt_y_coords;


Light  *SceneEditor::m_edit_selected_light = nullptr;
Object *SceneEditor::m_edit_selected_object = nullptr;

SceneEditor::SceneEditor() :GUI( 0 )
{  
}
SceneEditor::~SceneEditor()
{  
}

bool SceneEditor::initialize()
{  
  Button * bt_2 = GUI::getButton();
  bt_2->intitialize("resources/collapse.png","",glm::vec2(Engine::getWindowWidth() -40.0f,Engine::getWindowHeight()-40.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "expand_edit");

  m_back_panel_edit = GUI::getImage();
  m_back_panel_edit->load("resources/panel.png");
  m_back_panel_edit->setPosition(glm::vec2(Engine::getWindowWidth()-200.0f,Engine::getWindowHeight() -540.0f));
  m_back_panel_edit->setInactive();


  m_txt_del_light = GUI::getText();
  m_txt_del_light->setPosition(glm::vec2(Engine::getWindowWidth() -170.0f,Engine::getWindowHeight()-470.0f));
  m_txt_del_light->setScale(0.2f);
  m_txt_del_light->setColor(glm::vec3(0,0,0));
  m_txt_del_light->setText("Delete Light");
  m_txt_del_light->setInactive();

  m_btn_del_light = GUI::getButton();
  m_btn_del_light->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-470.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "delete_light");
  m_btn_del_light->setInactive();

  m_txt_add_light = GUI::getText();
  m_txt_add_light->setPosition(glm::vec2(Engine::getWindowWidth() -170.0f,Engine::getWindowHeight()-450.0f));
  m_txt_add_light->setScale(0.2f);
  m_txt_add_light->setColor(glm::vec3(0,0,0));
  m_txt_add_light->setText("Add Point-Light");
  m_txt_add_light->setInactive();

  m_btn_add_light = GUI::getButton();
  m_btn_add_light->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-450.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "add_light");
  m_btn_add_light->setInactive();

  m_txt_add_spot_light = GUI::getText();
  m_txt_add_spot_light->setPosition(glm::vec2(Engine::getWindowWidth() -170.0f,Engine::getWindowHeight()-430.0f));
  m_txt_add_spot_light->setScale(0.2f);
  m_txt_add_spot_light->setColor(glm::vec3(0,0,0));
  m_txt_add_spot_light->setText("Add Spot-Light");
  m_txt_add_spot_light->setInactive();

  m_btn_add_spot_light = GUI::getButton();
  m_btn_add_spot_light->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-430.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "add_spot_light");
  m_btn_add_spot_light->setInactive();

  m_txt_add_object = GUI::getText();
  m_txt_add_object->setPosition(glm::vec2(Engine::getWindowWidth() -170.0f,Engine::getWindowHeight()-400.0f));
  m_txt_add_object->setScale(0.2f);
  m_txt_add_object->setColor(glm::vec3(0,0,0));
  m_txt_add_object->setText("Add Object");
  m_txt_add_object->setInactive();

  m_btn_add_object = GUI::getButton();
  m_btn_add_object->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-400.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "add_object");
  m_btn_add_object->setInactive();



  m_btn_x_plus = GUI::getButton();
  m_btn_x_plus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -175.0f,Engine::getWindowHeight()-68.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "x_plus");
  m_btn_x_plus->setInactive();
  m_btn_x_minus = GUI::getButton();
  m_btn_x_minus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -175.0f,Engine::getWindowHeight()-93.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "x_minus");
  m_btn_x_minus->setInactive();

  m_btn_z_plus = GUI::getButton();
  m_btn_z_plus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-80.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "z_plus");
  m_btn_z_plus->setInactive();
  m_btn_z_minus = GUI::getButton();
  m_btn_z_minus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -160.0f,Engine::getWindowHeight()-80.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "z_minus");
  m_btn_z_minus->setInactive();

  m_btn_y_plus = GUI::getButton();
  m_btn_y_plus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -140.0f,Engine::getWindowHeight()-68.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "y_plus");
  m_btn_y_plus->setInactive();
  m_btn_y_minus = GUI::getButton();
  m_btn_y_minus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -140.0f,Engine::getWindowHeight()-93.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "y_minus");
  m_btn_y_minus->setInactive();

  m_txt_xz_coords = GUI::getText();
  m_txt_xz_coords->setPosition(glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-30.0f));
  m_txt_xz_coords->setScale(0.25f);
  m_txt_xz_coords->setColor(glm::vec3(0,0,0));
  m_txt_xz_coords->setText("");
  m_txt_xz_coords->setInactive();

  m_txt_y_coords = GUI::getText();
  m_txt_y_coords->setPosition(glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-40.0f));
  m_txt_y_coords->setScale(0.25f);
  m_txt_y_coords->setColor(glm::vec3(0,0,0));
  m_txt_y_coords->setText("");
  m_txt_y_coords->setInactive();


  GUI::registerButtonPressedCallback(mouseButtonCallback);
  Engine::getLog()->log("SceneEditor", "initialized");

  return true;
}

void SceneEditor::mouseButtonCallback(Button * btn)
{
  if(btn->getDescriptor() == "expand_edit")
  {
    btn->setImage("resources/expand.png");
    btn->setDescriptor("collapse_edit");
    btn->setPositionAndScale(glm::vec2(Engine::getWindowWidth() -240.0f,Engine::getWindowHeight()-40.0f),glm::vec2(0.5f,0.5f));    
    m_back_panel_edit->setActive();
    m_btn_del_light->setActive();
    m_txt_del_light->setActive();
    m_btn_add_light->setActive();
    m_txt_add_light->setActive();
    m_btn_add_spot_light->setActive();
    m_txt_add_spot_light->setActive();
    m_btn_x_plus->setActive();
    m_btn_x_minus->setActive();
    m_btn_y_plus->setActive();
    m_btn_y_minus->setActive();
    m_btn_z_plus->setActive();
    m_btn_z_minus->setActive();
    m_txt_xz_coords->setActive();
    m_txt_y_coords->setActive();
    m_btn_add_object->setActive();
    m_txt_add_object->setActive();
  }
  else
  {
    if(btn->getDescriptor() == "collapse_edit")
    {
      btn->setImage("resources/collapse.png");
      btn->setDescriptor("expand_edit");
      btn->setPositionAndScale(glm::vec2(Engine::getWindowWidth() -40.0f,Engine::getWindowHeight()-40.0f),glm::vec2(0.5f,0.5f));

      m_back_panel_edit->setInactive();
      m_btn_del_light->setInactive();
      m_txt_del_light->setInactive();
      m_btn_add_light->setInactive();
      m_txt_add_light->setInactive();
      m_btn_add_spot_light->setInactive();
      m_txt_add_spot_light->setInactive();
      m_btn_x_plus->setInactive();
      m_btn_x_minus->setInactive();
      m_btn_y_plus->setInactive();
      m_btn_y_minus->setInactive();
      m_btn_z_plus->setInactive();
      m_btn_z_minus->setInactive();
      m_txt_xz_coords->setInactive();
      m_txt_y_coords->setInactive();
      m_btn_add_object->setInactive();
      m_txt_add_object->setInactive();
    }
  }
  if(btn->getDescriptor() == "delete_light")
  {
    if(m_edit_selected_light != nullptr)
    {
      Engine::getScene()->removeLight(m_edit_selected_light);
      m_edit_selected_light = nullptr;
      m_txt_xz_coords->setText("Position:");
      m_txt_y_coords->setText("Height:");
    }
  }
  if(btn->getDescriptor() == "add_light")
  {    
    Light * new_light =  Engine::getScene()->addLight();
    if( !new_light->setPoint(Engine::getScene()->getCamera()->getPosition(),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),0.5f,0.1f,0.09f,0.032f,true) )
      Engine::getScene()->removeLight(new_light);
  }
  if(btn->getDescriptor() == "add_spot_light")
  {    
    Light * new_light =  Engine::getScene()->addLight();
    if( !new_light->setSpot(Engine::getScene()->getCamera()->getPosition(),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1), 1.0f,1.0f,0.09f,0.032f, 45.0f, glm::vec2(0,0), true ))
      Engine::getScene()->removeLight(new_light);    
  }
  if(btn->getDescriptor() == "x_plus")
  {
    moveActive(glm::vec3(1,0,0));    
  }
  if(btn->getDescriptor() == "x_minus")
  {
   moveActive(glm::vec3(-1,0,0));    
  }
  if(btn->getDescriptor() == "y_plus")
  {
    moveActive(glm::vec3(0,1,0));    
  }
  if(btn->getDescriptor() == "y_minus")
  {
    moveActive(glm::vec3(0,-1,0));    
  }
   if(btn->getDescriptor() == "z_plus")
  {
    moveActive(glm::vec3(0,0,1));    
  }
  if(btn->getDescriptor() == "z_minus")
  {
    moveActive(glm::vec3(0,0,-1));    
  }
  if(btn->getDescriptor() == "add_object")
  {
  }
}

void SceneEditor::moveActive(glm::vec3 dir)
{
  if(m_edit_selected_light != nullptr)
  {
    float factor = 0.05f;
    dir *= factor;
    m_edit_selected_light->move(dir);
    glm::vec3 p = m_edit_selected_light->getPosition();
    std::stringstream ss,ss2;
    ss<< std::setprecision(2)<< std::fixed<< "Position: "<< p.x <<" , "<<p.z  ;
    ss2<< std::setprecision(2)<< std::fixed<<"Height:   "<< p.y ;
    m_txt_xz_coords->setText(ss.str());
    m_txt_y_coords->setText(ss2.str());
  }
  if(m_edit_selected_object != nullptr)
  {
    float factor = 0.05f;
    dir *= factor;
    m_edit_selected_object->move(dir);
    glm::vec3 p = m_edit_selected_object->getPosition();
    std::stringstream ss,ss2;
    ss<< std::setprecision(2)<< std::fixed<< "Position: "<< p.x <<" , "<<p.z  ;
    ss2<< std::setprecision(2)<< std::fixed<<"Height:   "<< p.y ;
    m_txt_xz_coords->setText(ss.str());
    m_txt_y_coords->setText(ss2.str());
    Engine::getScene()->refreshLights();
  }
}

void SceneEditor::update()
{
  if( (m_edit_selected_light != nullptr) || (m_edit_selected_object != nullptr) )
  {
    if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_8) )
    {
      moveActive(glm::vec3(1,0,0));
    }
    if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_2) )
    {
      moveActive(glm::vec3(-1,0,0));
    }
    if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_4 ) )
    {
      moveActive(glm::vec3(0,0,1));
    }
    if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_6) )
    {
      moveActive(glm::vec3(0,0,-1));
    }
    if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_ADD ) )
    {
      moveActive(glm::vec3(0,1,0));
    }
    if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_SUBTRACT) )
    {
      moveActive(glm::vec3(0,-1,0));
    }
    if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_9) )
    {
      if(m_edit_selected_light != nullptr)
      {
        if(m_edit_selected_light->getType() == LIGHT_SPOT)
          m_edit_selected_light->rotate(glm::vec2(1.0f,0.0f));
      }
      if(m_edit_selected_object != nullptr)      
        m_edit_selected_object->rotate(glm::vec3(1,0,0));
      
    }
    if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_7 ) )
    {
      if(m_edit_selected_light != nullptr)
      {
        if(m_edit_selected_light->getType() == LIGHT_SPOT)
          m_edit_selected_light->rotate(glm::vec2(-1.0f,0.0f));
      }
      if(m_edit_selected_object != nullptr)      
        m_edit_selected_object->rotate(glm::vec3(-1,0,0));
    }
    if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_1) )
    {
      if(m_edit_selected_light != nullptr)
      {
        if(m_edit_selected_light->getType() == LIGHT_SPOT)
          m_edit_selected_light->rotate(glm::vec2(0.0f,1.0f));
      }
      if(m_edit_selected_object != nullptr)      
        m_edit_selected_object->rotate(glm::vec3(0,1,0));
    }
    if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_3 ) )
    {
      if(m_edit_selected_light != nullptr)
      {
        if(m_edit_selected_light->getType() == LIGHT_SPOT)
          m_edit_selected_light->rotate(glm::vec2(0.0f,-1.0f));
      
      }
      if(m_edit_selected_object != nullptr)      
        m_edit_selected_object->rotate(glm::vec3(0,-1,0));
    }
    if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_0 ) )
    {
      if(m_edit_selected_object != nullptr)      
        m_edit_selected_object->rotate(glm::vec3(0,0,1));
    }
     if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_DECIMAL ) )
    {
      if(m_edit_selected_object != nullptr)      
        m_edit_selected_object->rotate(glm::vec3(0,0,-1));
    }
  }

  if(IOSubsystem::isMouseButtonPressed(0) )
  {
    glm::vec2 p = IOSubsystem::getMouseCoordinates();
    if( (p.x < Engine::getWindowWidth() -200.0f) ||  (! m_back_panel_edit->isActive() ) )
    {
      //std::cout << "btn pressed pos: "<<p.x<<","<<p.y<<std::endl;
      Light * l = Engine::pickLightAt(p );
      if( l  != nullptr)   
      { 
        std::cout << "light "<<l->getId()<<" picked" <<std::endl;    
        m_edit_selected_light = l;
        glm::vec3 p = m_edit_selected_light->getPosition();
        std::stringstream ss,ss2;
        ss<< std::setprecision(2)<< std::fixed<< "Position: "<< p.x <<" , "<<p.z  ;
        ss2<< std::setprecision(2)<< std::fixed<<"Height:   "<< p.y ;
        m_txt_xz_coords->setText(ss.str());
        m_txt_y_coords->setText(ss2.str());
        m_edit_selected_object = nullptr;
      }
      Object * obj = Engine::pickObjectAt(p );
      if( obj  != nullptr)   
      { 
        std::cout << "Object "<<obj->getId()<<" picked" <<std::endl;    
        m_edit_selected_object= obj;
        glm::vec3 p = m_edit_selected_object->getPosition();
        std::stringstream ss,ss2;
        ss<< std::setprecision(2)<< std::fixed<< "Position: "<< p.x <<" , "<<p.z  ;
        ss2<< std::setprecision(2)<< std::fixed<<"Height:   "<< p.y ;
        m_txt_xz_coords->setText(ss.str());
        m_txt_y_coords->setText(ss2.str());
        m_edit_selected_light = nullptr;
      }
    }
  }
}
