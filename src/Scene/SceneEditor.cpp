#include "SceneEditor.h"

#include "IOSubsystem.h"
#include "Button.h"
#include "Image.h"
#include "Text.h"
#include "EditText.h"
#include "FileViewer.h"

#include "Engine.h"
#include "Scene.h"
#include "Camera.h"
#include "Log.h"
#include "Helper.h"

#include "Light.h"
#include "Object.h"

#include <iostream>
#include <sstream>
#include <iomanip>  

Image * SceneEditor::m_back_panel_edit;

Button * SceneEditor::m_btn_open_scene;
Button * SceneEditor::m_btn_save_scene;

Button * SceneEditor::m_btn_del_light;
Text * SceneEditor::m_txt_del_light;

Button * SceneEditor::m_btn_del_object;
Text * SceneEditor::m_txt_del_object;

Button * SceneEditor::m_btn_add_light;
Text * SceneEditor::m_txt_add_light;

Button * SceneEditor::m_btn_add_spot_light;
Text * SceneEditor::m_txt_add_spot_light;

Button * SceneEditor::m_btn_add_directional_light;
Text * SceneEditor::m_txt_add_directional_light;

Button * SceneEditor::m_btn_add_object;
Text * SceneEditor::m_txt_add_object;

Button * SceneEditor::m_btn_add_object_static;
Text * SceneEditor::m_txt_add_object_static;




Button * SceneEditor::m_btn_add_skybox;
Text * SceneEditor::m_txt_add_skybox;

EditText * SceneEditor::m_edittxt_x_coord;
EditText * SceneEditor::m_edittxt_y_coord;
EditText * SceneEditor::m_edittxt_z_coord;

EditText * SceneEditor::m_edittxt_x_rot;
EditText * SceneEditor::m_edittxt_y_rot;
EditText * SceneEditor::m_edittxt_z_rot;
EditText * SceneEditor::m_edittxt_scale;

EditText * SceneEditor::m_edittxt_light_color;
Text * SceneEditor::m_txt_light_color;

Button * SceneEditor::m_btn_x_plus;
Button * SceneEditor::m_btn_x_minus;
Button * SceneEditor::m_btn_y_plus;
Button * SceneEditor::m_btn_y_minus;
Button * SceneEditor::m_btn_z_plus;
Button * SceneEditor::m_btn_z_minus;

Button * SceneEditor::m_btn_rot_x_plus;
Button * SceneEditor::m_btn_rot_x_minus;
Button * SceneEditor::m_btn_rot_y_plus;
Button * SceneEditor::m_btn_rot_y_minus;
Button * SceneEditor::m_btn_rot_z_plus;
Button * SceneEditor::m_btn_rot_z_minus;

Button * SceneEditor::m_btn_scale_plus;
Button * SceneEditor::m_btn_scale_minus;


Text * SceneEditor::m_txt_coords;
Text * SceneEditor::m_txt_rotation;

FileViewer * SceneEditor::m_file_chooser;


Light  *SceneEditor::m_edit_selected_light = nullptr;
Object *SceneEditor::m_edit_selected_object = nullptr;

bool SceneEditor::m_expanded =false;
bool SceneEditor::m_just_activated = false;

SceneEditor::SceneEditor() :GUI( 0 )
{  
}

SceneEditor::~SceneEditor()
{  
  delete m_file_chooser;
}


//  initialization -------------------------------------------------

bool SceneEditor::initialize()
{  
  m_file_chooser = new FileViewer();
  if(! m_file_chooser->initialize() )
    return false;
 
  addChild((GUI*)m_file_chooser);


  Button * bt_2 = GUI::addButton();
  bt_2->intitialize("resources/collapse.png","",glm::vec2(Engine::getWindowWidth() -40.0f,Engine::getWindowHeight()-40.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "expand_edit");

  m_back_panel_edit = GUI::addImage();
  m_back_panel_edit->load("resources/panel.png");
  m_back_panel_edit->setPosition(glm::vec2(Engine::getWindowWidth()-200.0f,Engine::getWindowHeight() -540.0f));
  m_back_panel_edit->setInactive();


  m_btn_open_scene = GUI::addButton();
  m_btn_open_scene->intitialize("resources/btn_large_border.png","Open Scene",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-30.0f),glm::vec2(0.42f,0.5f),
    glm::vec3(0,0,0), "open_scene");
  m_btn_open_scene->setInactive();

  m_btn_save_scene = GUI::addButton();
  m_btn_save_scene->intitialize("resources/btn_large_border.png","SaveScene",glm::vec2(Engine::getWindowWidth() -90.0f,Engine::getWindowHeight()-30.0f),glm::vec2(0.42f,0.5f),
    glm::vec3(0,0,0), "save_scene");
  m_btn_save_scene->setInactive();


  m_txt_del_light = GUI::addText();
  m_txt_del_light->setPosition(glm::vec2(Engine::getWindowWidth() -170.0f,Engine::getWindowHeight()-490.0f));
  m_txt_del_light->setScale(0.22f);
  m_txt_del_light->setColor(glm::vec3(0,0,0));
  m_txt_del_light->setText("Delete Light");
  m_txt_del_light->setInactive();

  m_btn_del_light = GUI::addButton();
  m_btn_del_light->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-490.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "delete_light");
  m_btn_del_light->setInactive();


  m_txt_add_directional_light = GUI::addText();
  m_txt_add_directional_light->setPosition(glm::vec2(Engine::getWindowWidth() -170.0f,Engine::getWindowHeight()-470.0f));
  m_txt_add_directional_light->setScale(0.22f);
  m_txt_add_directional_light->setColor(glm::vec3(0,0,0));
  m_txt_add_directional_light->setText("Add Directional Light");
  m_txt_add_directional_light->setInactive();

  m_btn_add_directional_light = GUI::addButton();
  m_btn_add_directional_light->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-470.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "add_directional_light");
  m_btn_add_directional_light->setInactive();



  m_txt_add_light = GUI::addText();
  m_txt_add_light->setPosition(glm::vec2(Engine::getWindowWidth() -170.0f,Engine::getWindowHeight()-450.0f));
  m_txt_add_light->setScale(0.22f);
  m_txt_add_light->setColor(glm::vec3(0,0,0));
  m_txt_add_light->setText("Add Point-Light");
  m_txt_add_light->setInactive();

  m_btn_add_light = GUI::addButton();
  m_btn_add_light->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-450.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "add_light");
  m_btn_add_light->setInactive();

  m_txt_add_spot_light = GUI::addText();
  m_txt_add_spot_light->setPosition(glm::vec2(Engine::getWindowWidth() -170.0f,Engine::getWindowHeight()-430.0f));
  m_txt_add_spot_light->setScale(0.22f);
  m_txt_add_spot_light->setColor(glm::vec3(0,0,0));
  m_txt_add_spot_light->setText("Add Spot-Light");
  m_txt_add_spot_light->setInactive();

  m_btn_add_spot_light = GUI::addButton();
  m_btn_add_spot_light->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-430.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "add_spot_light");
  m_btn_add_spot_light->setInactive();


  m_txt_add_object_static = GUI::addText();
  m_txt_add_object_static->setPosition(glm::vec2(Engine::getWindowWidth() -170.0f,Engine::getWindowHeight()-365.0f));
  m_txt_add_object_static->setScale(0.22f);
  m_txt_add_object_static->setColor(glm::vec3(0,0,0));
  m_txt_add_object_static->setText("Add static  Object");
  m_txt_add_object_static->setInactive();

  m_btn_add_object_static = GUI::addButton();
  m_btn_add_object_static->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-365.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "add_object_static");
  m_btn_add_object_static->setInactive();


  m_txt_add_object = GUI::addText();
  m_txt_add_object->setPosition(glm::vec2(Engine::getWindowWidth() -170.0f,Engine::getWindowHeight()-385.0f));
  m_txt_add_object->setScale(0.22f);
  m_txt_add_object->setColor(glm::vec3(0,0,0));
  m_txt_add_object->setText("Add dynamic Object");
  m_txt_add_object->setInactive();

  m_btn_add_object = GUI::addButton();
  m_btn_add_object->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-385.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "add_object");
  m_btn_add_object->setInactive();

  m_txt_del_object = GUI::addText();
  m_txt_del_object->setPosition(glm::vec2(Engine::getWindowWidth() -170.0f,Engine::getWindowHeight()-405.0f));
  m_txt_del_object->setScale(0.22f);
  m_txt_del_object->setColor(glm::vec3(0,0,0));
  m_txt_del_object->setText("Delete Object");
  m_txt_del_object->setInactive();

  m_btn_del_object = GUI::addButton();
  m_btn_del_object->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-405.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "delete_object");
  m_btn_del_object->setInactive();



  m_txt_add_skybox = GUI::addText();
  m_txt_add_skybox->setPosition(glm::vec2(Engine::getWindowWidth() -170.0f,Engine::getWindowHeight()-515.0f));
  m_txt_add_skybox->setScale(0.22f);
  m_txt_add_skybox->setColor(glm::vec3(0,0,0));
  m_txt_add_skybox->setText("Add Skybox");
  m_txt_add_skybox->setInactive();

  m_btn_add_skybox = GUI::addButton();
  m_btn_add_skybox->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-515.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "add_skybox");
  m_btn_add_skybox->setInactive();


 
  m_txt_coords = GUI::addText();
  m_txt_coords->setPosition(glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-50.0f));
  m_txt_coords->setScale(0.25f);
  m_txt_coords->setColor(glm::vec3(0,0,0));
  m_txt_coords->setText("Position");
  m_txt_coords->setInactive();


  // x edit
  m_edittxt_x_coord = GUI::addEditText();
  m_edittxt_x_coord->set( "x", glm::vec2(Engine::getWindowWidth() -120.0f,Engine::getWindowHeight()-65.0f), 0.22f, glm::vec3(0,0,0));
  m_edittxt_x_coord->setInactive();
  m_edittxt_x_coord->setDescriptor("x_coord");

  m_btn_x_plus = GUI::addButton();
  m_btn_x_plus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -175.0f,Engine::getWindowHeight()-68.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "x_plus");
  m_btn_x_plus->setInactive();
  m_btn_x_minus = GUI::addButton();
  m_btn_x_minus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -150.0f,Engine::getWindowHeight()-68.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "x_minus");
  m_btn_x_minus->setInactive();

  // y edit
  m_edittxt_y_coord = GUI::addEditText();
  m_edittxt_y_coord->set( "y", glm::vec2(Engine::getWindowWidth() -120.0f,Engine::getWindowHeight()-80.0f), 0.22f, glm::vec3(0,0,0));
  m_edittxt_y_coord->setInactive();
  m_edittxt_y_coord->setDescriptor("y_coord");

  m_btn_y_plus = GUI::addButton();
  m_btn_y_plus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -175.0f,Engine::getWindowHeight()-83.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "y_plus");
  m_btn_y_plus->setInactive();
  m_btn_y_minus = GUI::addButton();
  m_btn_y_minus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -150.0f,Engine::getWindowHeight()-83.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "y_minus");
  m_btn_y_minus->setInactive();

  // z edit
  m_edittxt_z_coord = GUI::addEditText();
  m_edittxt_z_coord->set( "z", glm::vec2(Engine::getWindowWidth() -120.0f,Engine::getWindowHeight()-95.0f), 0.22f, glm::vec3(0,0,0));
  m_edittxt_z_coord->setInactive();
  m_edittxt_z_coord->setDescriptor("z_coord");

  m_btn_z_plus = GUI::addButton();
  m_btn_z_plus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -175.0f,Engine::getWindowHeight()-98.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "z_plus");
  m_btn_z_plus->setInactive();
  m_btn_z_minus = GUI::addButton();
  m_btn_z_minus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -150.0f,Engine::getWindowHeight()-98.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "z_minus");
  m_btn_z_minus->setInactive();



  m_txt_rotation = GUI::addText();
  m_txt_rotation->setPosition(glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-120.0f));
  m_txt_rotation->setScale(0.25f);
  m_txt_rotation->setColor(glm::vec3(0,0,0));
  m_txt_rotation->setText("Rotation");
  m_txt_rotation->setInactive();

  // rot 1 edit
  m_edittxt_x_rot = GUI::addEditText();
  m_edittxt_x_rot->set( "x rot", glm::vec2(Engine::getWindowWidth() -120.0f,Engine::getWindowHeight()-140.0f), 0.22f, glm::vec3(0,0,0));
  m_edittxt_x_rot->setInactive();
  m_edittxt_x_rot->setDescriptor("x_rot");

  m_btn_rot_x_plus = GUI::addButton();
  m_btn_rot_x_plus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -175.0f,Engine::getWindowHeight()-143.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "x_rot_plus");
  m_btn_rot_x_plus->setInactive();
  m_btn_rot_x_minus = GUI::addButton();
  m_btn_rot_x_minus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -150.0f,Engine::getWindowHeight()-143.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "x_rot_minus");
  m_btn_rot_x_minus->setInactive();

  // rot y edit
  m_edittxt_y_rot = GUI::addEditText();
  m_edittxt_y_rot->set( "y rot", glm::vec2(Engine::getWindowWidth() -120.0f,Engine::getWindowHeight()-155.0f), 0.22f, glm::vec3(0,0,0));
  m_edittxt_y_rot->setInactive();
  m_edittxt_y_rot->setDescriptor("y_rot");

  m_btn_rot_y_plus = GUI::addButton();
  m_btn_rot_y_plus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -175.0f,Engine::getWindowHeight()-158.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "y_rot_plus");
  m_btn_rot_y_plus->setInactive();
  m_btn_rot_y_minus = GUI::addButton();
  m_btn_rot_y_minus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -150.0f,Engine::getWindowHeight()-158.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "y_rot_minus");
  m_btn_rot_y_minus->setInactive();

  // rot z edit
  m_edittxt_z_rot = GUI::addEditText();
  m_edittxt_z_rot->set( "z rot", glm::vec2(Engine::getWindowWidth() -120.0f,Engine::getWindowHeight()-170.0f), 0.22f, glm::vec3(0,0,0));
  m_edittxt_z_rot->setInactive();
  m_edittxt_z_rot->setDescriptor("z_rot");

  m_btn_rot_z_plus = GUI::addButton();
  m_btn_rot_z_plus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -175.0f,Engine::getWindowHeight()-173.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "z_rot_plus");
  m_btn_rot_z_plus->setInactive();
  m_btn_rot_z_minus = GUI::addButton();
  m_btn_rot_z_minus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -150.0f,Engine::getWindowHeight()-173.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "z_rot_minus");
  m_btn_rot_z_minus->setInactive();

  // scale 
  m_edittxt_scale = GUI::addEditText();
  m_edittxt_scale->set( "scale", glm::vec2(Engine::getWindowWidth() -40.0f,Engine::getWindowHeight()-140.0f), 0.22f, glm::vec3(0,0,0));
  m_edittxt_scale->setInactive();
  m_edittxt_scale->setDescriptor("scale");

  m_btn_scale_plus = GUI::addButton();
  m_btn_scale_plus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -95.0f,Engine::getWindowHeight()-143.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "scale_plus");
  m_btn_scale_plus->setInactive();
  m_btn_scale_minus = GUI::addButton();
  m_btn_scale_minus->intitialize("resources/btn.png","",glm::vec2(Engine::getWindowWidth() -70.0f,Engine::getWindowHeight()-143.0f),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "scale_minus");
  m_btn_scale_minus->setInactive();


  m_edittxt_light_color = GUI::addEditText();
  m_edittxt_light_color->set( "RGB", glm::vec2(Engine::getWindowWidth() -120.0f,Engine::getWindowHeight()-190.0f), 0.22f, glm::vec3(0,0,0));
  m_edittxt_light_color->setInactive();
  m_edittxt_light_color->setDescriptor("light_color");

  m_txt_light_color = GUI::addText();
  m_txt_light_color->setPosition(glm::vec2(Engine::getWindowWidth() -190.0f,Engine::getWindowHeight()-190.0f));
  m_txt_light_color->setScale(0.25f);
  m_txt_light_color->setColor(glm::vec3(0,0,0));
  m_txt_light_color->setText("Color:");
  m_txt_light_color->setInactive();


  GUI::registerButtonPressedCallback(mouseButtonCallback);
  GUI::registerEditTextCallback(editTextChangedCallback);
  Engine::getLog()->log("SceneEditor", "initialized");

  return true;
}


//  getter/setter -------------------------------------------------

void SceneEditor::setLightSpecifics(bool on)
{  
  if(on)
  {
    m_edittxt_light_color->setActive();
    m_txt_light_color->setActive();   
  }
  else
  {
    m_edittxt_light_color->setInactive();
    m_txt_light_color->setInactive();
  }
}

void SceneEditor::setObjectSpecifics(bool on)
{  
  if(on)
  {
    m_edittxt_z_rot->setActive();
    m_btn_rot_z_minus->setActive();
    m_btn_rot_z_plus->setActive();
    m_edittxt_scale->setActive();
  }
  else
  {
    m_edittxt_z_rot->setInactive();
    m_btn_rot_z_minus->setInactive();
    m_btn_rot_z_plus->setInactive();
    m_edittxt_scale->setInactive();
  }
}


//  callbacks -------------------------------------------------

void SceneEditor::editTextChangedCallback(EditText * edit_txt)
{
  Engine::getLog()->log("SceneEditor", "edit text finished");
  if( edit_txt->getDescriptor() == "x_coord"  )
  {
    std::istringstream iss(edit_txt->getText());
    float f;
    iss >> f;
    if( iss.eof() && !iss.fail())
    {
      float c = ::atof(edit_txt->getText().c_str());
      if(m_edit_selected_object != nullptr)
      {
        glm::vec3 p = m_edit_selected_object->getPosition();
        p.x= c;
        m_edit_selected_object->setPosition(p);
      }
      if(m_edit_selected_light != nullptr)
      {
        glm::vec3 p = m_edit_selected_light->getPosition();
        p.x= c;
        m_edit_selected_light->setPosition(p);
      }
    }
    else
    {
      glm::vec3 p;
      glm::vec3 r;
      if(m_edit_selected_object != nullptr)
      {
        p = m_edit_selected_object->getPosition();
        r = m_edit_selected_object->getRotation();
      }
      if(m_edit_selected_light != nullptr)
      {
        p = m_edit_selected_light->getPosition();
        r = m_edit_selected_light->getDirection();
      }
      refreshText(p,r);
    }
  }
  if( edit_txt->getDescriptor() == "y_coord"  )
  {
    std::istringstream iss(edit_txt->getText());
    float f;
    iss >> f;
    if( iss.eof() && !iss.fail())
    {
      float c = ::atof(edit_txt->getText().c_str());
      if(m_edit_selected_object != nullptr)
      {
        glm::vec3 p = m_edit_selected_object->getPosition();
        p.y= c;
        m_edit_selected_object->setPosition(p);
      }
      if(m_edit_selected_light != nullptr)
      {
        glm::vec3 p = m_edit_selected_light->getPosition();
        p.y= c;
        m_edit_selected_light->setPosition(p);
      }
    }
    else
    {
      glm::vec3 p;
      glm::vec3 r;
      if(m_edit_selected_object != nullptr)
      {
        p = m_edit_selected_object->getPosition();
        r = m_edit_selected_object->getRotation();
      }
      if(m_edit_selected_light != nullptr)
      {
        p = m_edit_selected_light->getPosition();
        r = m_edit_selected_light->getDirection();
      }
      refreshText(p,r);
    }
  }
  if( edit_txt->getDescriptor() == "z_coord"  )
  {
    std::istringstream iss(edit_txt->getText());
    float f;
    iss >> f;
    if( iss.eof() && !iss.fail())
    {
      float c = ::atof(edit_txt->getText().c_str());
      if(m_edit_selected_object != nullptr)
      {
        glm::vec3 p = m_edit_selected_object->getPosition();
        p.z= c;
        m_edit_selected_object->setPosition(p);
      }
      if(m_edit_selected_light != nullptr)
      {
        glm::vec3 p = m_edit_selected_light->getPosition();
        p.z= c;
        m_edit_selected_light->setPosition(p);
      }
    }
    else
    {
      glm::vec3 p;
      glm::vec3 r;
      if(m_edit_selected_object != nullptr)
      {
        p = m_edit_selected_object->getPosition();
        r = m_edit_selected_object->getRotation();
      }
      if(m_edit_selected_light != nullptr)
      {
        p = m_edit_selected_light->getPosition();
        r = m_edit_selected_light->getDirection();
      }
      refreshText(p,r);
    }
  }
  if( edit_txt->getDescriptor() == "x_rot"  )
  {
    std::istringstream iss(edit_txt->getText());
    float f;
    iss >> f;
    if( iss.eof() && !iss.fail())
    {
      float c = ::atof(edit_txt->getText().c_str());
      if(m_edit_selected_object != nullptr)
      {
        glm::vec3 r = m_edit_selected_object->getRotationDegrees();
        r.x= c;
        m_edit_selected_object->setRotation(r);
      }
      if(m_edit_selected_light != nullptr)
      {
        glm::vec2 r = m_edit_selected_light->getRotation();
        r.x= c;
        m_edit_selected_light->setRotation(r);
      }
    }
    else
    {
      glm::vec3 p;
      glm::vec3 r;
      if(m_edit_selected_object != nullptr)
      {
        p = m_edit_selected_object->getPosition();
        r = m_edit_selected_object->getRotationDegrees();
      }
      if(m_edit_selected_light != nullptr)
      {
        p = m_edit_selected_light->getPosition();
        glm::vec2 r2 = m_edit_selected_light->getRotation();
        r = glm::vec3(r2.x,r2.y,0);
      }
      refreshText(p,r);
    }
  }
  if( edit_txt->getDescriptor() == "y_rot"  )
  {
    std::istringstream iss(edit_txt->getText());
    float f;
    iss >> f;
    if( iss.eof() && !iss.fail())
    {
      float c = ::atof(edit_txt->getText().c_str());
      if(m_edit_selected_object != nullptr)
      {
        glm::vec3 r = m_edit_selected_object->getRotationDegrees();
        r.y= c;
        m_edit_selected_object->setRotation(r);
      }
      if(m_edit_selected_light != nullptr)
      {
        glm::vec2 r = m_edit_selected_light->getRotation();
        r.y= c;
        m_edit_selected_light->setRotation(r);
      }
    }
    else
    {
      glm::vec3 p;
      glm::vec3 r;
      if(m_edit_selected_object != nullptr)
      {
        p = m_edit_selected_object->getPosition();
        r = m_edit_selected_object->getRotationDegrees();
      }
      if(m_edit_selected_light != nullptr)
      {
        p = m_edit_selected_light->getPosition();
        glm::vec2 r2 = m_edit_selected_light->getRotation();
        r = glm::vec3(r2.x,r2.y,0);
      }
      refreshText(p,r);
    }
  }
  if( edit_txt->getDescriptor() == "z_rot"  )
  {
    std::istringstream iss(edit_txt->getText());
    float f;
    iss >> f;
    if( iss.eof() && !iss.fail())
    {
      float c = ::atof(edit_txt->getText().c_str());
      if(m_edit_selected_object != nullptr)
      {
        glm::vec3 r = m_edit_selected_object->getRotationDegrees();
        r.z= c;
        m_edit_selected_object->setRotation(r);
      }
    }
    else
    {
      glm::vec3 p;
      glm::vec3 r;
      if(m_edit_selected_object != nullptr)
      {
        p = m_edit_selected_object->getPosition();
        r = m_edit_selected_object->getRotationDegrees();
      }
      if(m_edit_selected_light != nullptr)
      {
        p = m_edit_selected_light->getPosition();
        glm::vec2 r2 = m_edit_selected_light->getRotation();
        r = glm::vec3(r2.x,r2.y,0);
      }
      refreshText(p,r);
    }
  }
  if( edit_txt->getDescriptor() == "scale"  )
  {
    if(m_edit_selected_object != nullptr)
    {
      std::istringstream iss(edit_txt->getText());
      float f;
      iss >> f;
      if( iss.eof() && !iss.fail())
      {
        float c = ::atof(edit_txt->getText().c_str());
        if(m_edit_selected_object != nullptr)
        {
          glm::vec3 s =  glm::vec3(c,c,c);
          m_edit_selected_object->setScale(s);
        }
      }
      else
      {
        glm::vec3 p;
        glm::vec3 r;
        if(m_edit_selected_object != nullptr)
        {
          p = m_edit_selected_object->getPosition();
          r = m_edit_selected_object->getRotationDegrees();

          refreshText(p,r);
        }
      }
    }
  }
  if(edit_txt->getDescriptor() == "light_color")
  {
    if(m_edit_selected_light != nullptr)
    {
      std::vector<std::string> parts = Helper::split(edit_txt->getText(), ',');
      if(parts.size()  == 3 )
      {        
        bool valid = true;

        std::istringstream iss1(parts[0]);
        std::istringstream iss2(parts[1]);
        std::istringstream iss3(parts[2]);
        float f1,f2,f3;
        iss1 >> f1;
        iss2 >> f2;
        iss3 >> f3;
        if( ! (iss1.eof() && !iss1.fail()) )
          valid = false;
        if( ! (iss2.eof() && !iss2.fail()) )
          valid = false;
        if( ! (iss3.eof() && !iss3.fail()) )
          valid = false;

        if( valid && (f1 <=255.0f) && (f2 <=255.0f)&& (f3 <=255.0f) && (f1 >=0.0f)&& (f2 >=0.0f)&& (f3 >=0.0f)  )
        {
          m_edit_selected_light->setColor(glm::vec3(f1/255.0f,f2/255.0f,f3/255.0f));
        }
      }
    }
  }


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
    m_btn_add_directional_light->setActive();
    m_txt_add_directional_light->setActive();
    m_btn_x_plus->setActive();
    m_btn_x_minus->setActive();
    m_btn_y_plus->setActive();
    m_btn_y_minus->setActive();
    m_btn_z_plus->setActive();
    m_btn_z_minus->setActive();
    m_btn_scale_minus->setActive();
    m_btn_scale_plus->setActive();
    m_txt_coords->setActive();
    m_txt_rotation->setActive();
    m_btn_add_object->setActive();
    m_txt_add_object->setActive();
    m_btn_add_object_static->setActive();
    m_txt_add_object_static->setActive();
    m_btn_del_object->setActive();
    m_txt_del_object->setActive();
    m_btn_add_skybox->setActive();
    m_txt_add_skybox->setActive();

 
    m_edittxt_x_coord->setActive();
    m_edittxt_y_coord->setActive();
    m_edittxt_z_coord->setActive();

    m_edittxt_scale->setActive();

    m_edittxt_x_rot->setActive();
    m_btn_rot_x_minus->setActive();
    m_btn_rot_x_plus->setActive();
    m_edittxt_y_rot->setActive();
    m_btn_rot_y_minus->setActive();
    m_btn_rot_y_plus->setActive();
    if(m_edit_selected_object != nullptr)    
      setObjectSpecifics(true);
    if(m_edit_selected_light != nullptr)    
      setLightSpecifics(true);
    m_just_activated = true;
    m_expanded = true;
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
      m_btn_add_directional_light->setInactive();
      m_txt_add_directional_light->setInactive();
      m_btn_x_plus->setInactive();
      m_btn_x_minus->setInactive();
      m_btn_y_plus->setInactive();
      m_btn_y_minus->setInactive();
      m_btn_z_plus->setInactive();
      m_btn_z_minus->setInactive();
      m_btn_scale_minus->setInactive();
      m_btn_scale_plus->setInactive();
      m_txt_coords->setInactive();
      m_txt_rotation->setInactive();
      m_btn_add_object->setInactive();
      m_txt_add_object->setInactive();
      m_btn_add_object_static->setInactive();
      m_txt_add_object_static->setInactive();
      m_btn_del_object->setInactive();
      m_txt_del_object->setInactive();
      m_btn_add_skybox->setInactive();
      m_txt_add_skybox->setInactive();
      m_btn_open_scene->setInactive();
      m_btn_save_scene->setInactive();

      m_edittxt_x_coord->setInactive();
      m_edittxt_y_coord->setInactive();
      m_edittxt_z_coord->setInactive();

      m_edittxt_scale->setInactive();

      m_edittxt_x_rot->setInactive();
      m_btn_rot_x_minus->setInactive();
      m_btn_rot_x_plus->setInactive();
      m_edittxt_y_rot->setInactive();
      m_btn_rot_y_minus->setInactive();
      m_btn_rot_y_plus->setInactive();
      if(m_edit_selected_object != nullptr)    
        setObjectSpecifics(false);
      if(m_edit_selected_light != nullptr)    
        setLightSpecifics(false);
      m_expanded= false;
    }
  }
  if(btn->getDescriptor() == "delete_light")
  {
    if(m_edit_selected_light != nullptr)
    {
      Engine::getScene()->removeLight(m_edit_selected_light);
      m_edit_selected_light = nullptr;
      m_txt_coords->setText("Position:");
      m_txt_rotation->setText("Rotation:");
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
  if(btn->getDescriptor() == "add_directional_light")
  {    
    Light * new_light =  Engine::getScene()->addLight();
    if( !new_light->setDirectional(glm::vec3(0,1,0), glm::vec3(1,0.95,0.9) ,glm::vec3(1,1,1), glm::vec3(1,1,1),0.3f, true) )
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
  if(btn->getDescriptor() == "x_rot_plus")
  {
    rotateActive(glm::vec3(1,0,0));    
  }
  if(btn->getDescriptor() == "x_rot_minus")
  {
   rotateActive(glm::vec3(-1,0,0));    
  }
  if(btn->getDescriptor() == "y_rot_plus")
  {
    rotateActive(glm::vec3(0,1,0));    
  }
  if(btn->getDescriptor() == "y_rot_minus")
  {
    rotateActive(glm::vec3(0,-1,0));    
  }
   if(btn->getDescriptor() == "z_rot_plus")
  {
    rotateActive(glm::vec3(0,0,1));    
  }
  if(btn->getDescriptor() == "z_rot_minus")
  {
    rotateActive(glm::vec3(0,0,-1));    
  }
  if(btn->getDescriptor() == "scale_plus")
  {    
   if( m_edit_selected_object != nullptr) 
   {
      std::stringstream ss3;
      glm::vec3 s = m_edit_selected_object->getScale();
      s+= glm::vec3(0.05f,0.05f,0.05f);
      m_edit_selected_object->setScale(s);
      glm::vec3 c = m_edit_selected_object->getScale();
      ss3 <<c.x;
      m_edittxt_scale->setText(ss3.str());
   }
  }
  if(btn->getDescriptor() == "scale_minus")
  {
    if( m_edit_selected_object != nullptr) 
    {      
      std::stringstream ss3;
      glm::vec3 s = m_edit_selected_object->getScale();
      s-= glm::vec3(0.05f,0.05f,0.05f);
      m_edit_selected_object->setScale(s);
      glm::vec3 c = m_edit_selected_object->getScale();
      ss3 <<c.x;
      m_edittxt_scale->setText(ss3.str());
    }
  }
  if(btn->getDescriptor() == "delete_object")
  {
    if( m_edit_selected_object != nullptr) 
    { 
      Engine::getScene()->removeObject(m_edit_selected_object);
    }
  }
  
  if(btn->getDescriptor() == "add_object")
  {
    ( (SceneEditor* ) btn->getParent()  )->startObjectAdding(false);
  }
  if(btn->getDescriptor() == "add_object_static")
  {
    ( (SceneEditor* ) btn->getParent()  )->startObjectAdding(true);
  }
  if(btn->getDescriptor() == "add_skybox")
  {
    ( (SceneEditor* ) btn->getParent()  )->startSkyboxAdding();
  }
  if(btn->getDescriptor() == "open_scene")
  {
    ( (SceneEditor* ) btn->getParent()  )->startSceneOpening();
  }
  if(btn->getDescriptor() == "save_scene")
  {
     ( (SceneEditor* ) btn->getParent()  )->startSceneSaving();
  }
}


//  move/rotate -------------------------------------------------

void SceneEditor::rotateActive(glm::vec3 delta)
{
  if(! Engine::isGUIMovementLockSet())
  {
    if( (m_edit_selected_light != nullptr) && ( (delta.x != 0.0f) || (delta.y != 0.0f) ) )
    {
      float factor = 1.0f;
      delta *= factor;
      m_edit_selected_light->rotate(glm::vec2(delta.x,delta.y));
      glm::vec3 p = m_edit_selected_light->getPosition();
      glm::vec3 r = glm::vec3( m_edit_selected_light->getRotation().x,m_edit_selected_light->getRotation().y,0);

      //Engine::getScene()->refreshLights();
      m_edit_selected_light->refresh();
      refreshText(p,r);
    }
    if(m_edit_selected_object != nullptr)
    {
      float factor = 1.0f;
      delta *= factor;
      m_edit_selected_object->rotate(delta);
      glm::vec3 p = m_edit_selected_object->getPosition();
      glm::vec3 r = m_edit_selected_object->getRotationDegrees();

     // Engine::getScene()->refreshLights();
      refreshText(p,r);
    }
  }
}

void SceneEditor::moveActive(glm::vec3 dir)
{
  if(! Engine::isGUIMovementLockSet())
  {
    if(m_edit_selected_light != nullptr)
    {
      float factor = 0.05f;
      dir *= factor;
      m_edit_selected_light->move(dir);
      glm::vec3 p = m_edit_selected_light->getPosition();
      glm::vec3 r = glm::vec3( m_edit_selected_light->getRotation().x,m_edit_selected_light->getRotation().y,0);

      m_edit_selected_light->refresh();
     // Engine::getScene()->refreshLights();
      refreshText(p,r);
    }
    if(m_edit_selected_object != nullptr)
    {
      float factor = 0.05f;
      dir *= factor;
      m_edit_selected_object->move(dir);
      glm::vec3 p = m_edit_selected_object->getPosition();
      glm::vec3 r = m_edit_selected_object->getRotationDegrees();

    //  Engine::getScene()->refreshLights();
      refreshText(p,r);
    }
  }
}


//  dialogue start -------------------------------------------------

void SceneEditor::startObjectAdding(bool static_object )
{
  m_file_chooser->start();
  if(!static_object)
    m_file_chooser->setIdentifier("object_chooser");
  else
    m_file_chooser->setIdentifier("object_chooser_static");
}

void SceneEditor::startSceneOpening()
{
  m_file_chooser->start();
  m_file_chooser->setIdentifier("scene_chooser");
}

void SceneEditor::startSkyboxAdding()
{
  m_file_chooser->start();
  m_file_chooser->setIdentifier("skybox_chooser");
}

void SceneEditor::startSceneSaving()
{
  m_file_chooser->start();
  m_file_chooser->setIdentifier("scene_saver");
}


//  update/refresh -------------------------------------------------

void SceneEditor::update()
{
  if(m_just_activated)
  {
    m_btn_open_scene->setActive();
    m_btn_save_scene->setActive();
    m_just_activated = false;
  }
  if( (m_file_chooser->getStatus() != FC_STATUS_NOTACTIVE)  )
  {
    if(  m_file_chooser->getStatus() == FC_STATUS_DONE_OK   )
    {
      if(m_file_chooser->getIdentifier() == "object_chooser")
      {
        std::string file = m_file_chooser->getFileName();
        Object * new_object= Engine::getScene()->addObject(file,glm::vec3(0,0,0), false );  // TODO : change static flag accordingly      
      }
      if(m_file_chooser->getIdentifier() == "object_chooser_static")
      {
        std::string file = m_file_chooser->getFileName();
        Object * new_object= Engine::getScene()->addObject(file,glm::vec3(0,0,0), true );  // TODO : change static flag accordingly

      
      }
      if(m_file_chooser->getIdentifier() == "scene_chooser")
      {
        std::string file = m_file_chooser->getFileName();
        Engine::getLog()->log("SceneEditor", "loading scene");
        Scene *  new_scene = new Scene;
        if( new_scene->load(file) )
        { 
          Scene * old_scene =  Engine::getScene();         
          Engine::setScene(new_scene, true);     
        }
      }
      if(m_file_chooser->getIdentifier() == "skybox_chooser")
      {
        std::string file = m_file_chooser->getFileName();

        std::size_t found = file.find("_");
        if (found!=std::string::npos)
        {
          std::size_t pos = file.find_last_of("_");

          std::string f = file.substr(0,pos);
          std::string l = file.substr(pos+1);

          std::size_t lsf = l.find(".");
          std::string lb = l.substr(0,lsf);
          if( (lb== "dn") || (lb== "lf") || (lb== "bk") || (lb== "rt") || (lb== "up") || (lb== "ft")  )
          {
            file = f;

            if(! Engine::getScene()->setSkybox(file) )
              Engine::getLog()->log("SceneEditor", "couldn't set skybox");
          }
          else
            Engine::getLog()->log("SceneEditor", "skybox file name mismatch  (..._X.jpg) X element of {dn, lf, bk, rt, up, ft}");

        }
        else
            Engine::getLog()->log("SceneEditor", "skybox file name mismatch  (..._X.jpg) X element of {dn, lf, bk, rt, up, ft}");


      }
      if(m_file_chooser->getIdentifier() == "scene_saver")
      {
        Engine::getLog()->log("SceneEditor", "saving scene");
        std::string file = m_file_chooser->getFileName();
        if(file != "")
        {
          std::size_t found = file.find(".xml");
          if (found==std::string::npos)          
            file+= ".xml";
        }
        else
          file = "default_scene.xml";
        Engine::getScene()->save(file);
      }

      m_file_chooser->reset();
    }
    if(  m_file_chooser->getStatus() == FC_STATUS_DONE_CANCLED   )
    {
      if(m_file_chooser->getIdentifier() == "scene_chooser")
        Engine::getLog()->log("SceneEditor", "loading scene cancled");
      m_file_chooser->reset();
    }
  }  
  else
  {
    if(! Engine::isGUIMovementLockSet())
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
          rotateActive(glm::vec3(1.0f,0.0f,0.0f));          
        }
        if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_7 ) )
        {
          rotateActive(glm::vec3(-1.0f,0.0f,0.0f));  
        }
        if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_1) )
        {
          rotateActive(glm::vec3(0.0f,1.0f,0.0f));  
        }
        if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_3 ) )
        {
          rotateActive(glm::vec3(0.0f,-1.0f,0.0f));  
        }
        if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_0 ) )
        {
          rotateActive(glm::vec3(0.0f,0.0f,1.0f));  
        }
         if(IOSubsystem::isKeyPressed(GLFW_KEY_KP_DECIMAL ) )
        {
          rotateActive(glm::vec3(0.0f,0.0f,-1.0f));  
        }
      }

      if(IOSubsystem::isMouseButtonReleasedAndPressed(0) )
      {
        glm::vec2 p = IOSubsystem::getMouseCoordinates();
        if( ( (p.x < Engine::getWindowWidth() -200.0f) ||  (! m_back_panel_edit->isActive() ) )   && ! (m_file_chooser->isActive() && m_file_chooser->isInside(p))    )
        {
          
         //std::cout << "btn pressed pos: "<<p.x<<","<<p.y<<std::endl;
          Light * l = Engine::pickLightAt(p );
          if( l  != nullptr)   
          { 
            std::cout << "light "<<l->getId()<<" picked" <<std::endl;    
            m_edit_selected_light = l;
            glm::vec3 p = m_edit_selected_light->getPosition();
            glm::vec3 r = glm::vec3( m_edit_selected_light->getRotation().x,m_edit_selected_light->getRotation().y,0);
          
            m_edit_selected_object = nullptr;
            setObjectSpecifics(false);

            if(m_expanded)
            {
              setLightSpecifics(true);
            }

            refreshText(p,r);
    
          }
          Object * obj = Engine::pickObjectAt(p );
          if( obj  != nullptr)   
          { 
            std::cout << "Object "<<obj->getId()<<" picked" <<std::endl;    
            m_edit_selected_object= obj;
            glm::vec3 p = m_edit_selected_object->getPosition();
            glm::vec3 r = m_edit_selected_object->getRotationDegrees();
           
            m_edit_selected_light = nullptr;
            if(m_expanded)
            {   
              setObjectSpecifics(true);
            }
            setLightSpecifics(false);

            refreshText(p,r);
    
          }
        }
      }
    }    
  }
}

void SceneEditor::refreshText(glm::vec3 p, glm::vec3 r)
{
  std::stringstream ss3;
  ss3 << p.x;
  m_edittxt_x_coord->setText(ss3.str());
  ss3.str(""); ss3.clear();

  ss3 << p.y;
  m_edittxt_y_coord->setText(ss3.str());
  ss3.str(""); ss3.clear();

  ss3 << p.z;
  m_edittxt_z_coord->setText(ss3.str());
  ss3.str(""); ss3.clear();


  ss3 << r.x;
  m_edittxt_x_rot->setText(ss3.str());
  ss3.str(""); ss3.clear();

  ss3 << r.y;
  m_edittxt_y_rot->setText(ss3.str());
  ss3.str(""); ss3.clear();

  ss3 << r.z;
  m_edittxt_z_rot->setText(ss3.str());
  ss3.str(""); ss3.clear();

  if(m_edit_selected_light != nullptr)
  { 
    glm::vec3 c = m_edit_selected_light->getDiffuseColor();
    ss3 <<c.x*255.0f<<","<<c.y*255.0f<<","<<c.z*255.0f;
    m_edittxt_light_color->setText(ss3.str());

  }
  if(m_edit_selected_object != nullptr)
  { 
    glm::vec3 c = m_edit_selected_object->getScale();
    ss3 <<c.x;
    m_edittxt_scale->setText(ss3.str());
  }

}
