#include "Player.h"

#include "Camera.h"
#include "Scene.h"
#include "Object.h"
#include "Inventory.h"
#include "Item.h"
#include "SoundManager.h"
#include "Game.h"


#include "Engine.h"

#include "GUI.h"
#include "Text.h"
#include "Material.h"
#include "Model.h"



#include "Script.h"
#include "RotationScriptElement.h"

#define JUMP_TIME 0.1f
#define JUMP_CD 1.0f

#include <iostream>
#include <sstream>

Camera * Player::m_camera;
Inventory * Player::m_inventory;

Object * Player::m_player_object;

float Player::m_energy;

GUI * Player::m_gui;
Text * Player::m_lower_text;

int Player::m_jump_state;
float Player::m_jump_time;

unsigned int Player::m_movement_keys_pressed;




Player::Player()
{  
  m_energy = 100.0f;
  m_jump_time = 0.0f;
  m_jump_state = 0;
  m_movement_keys_pressed = 0;
}

Player::~Player()
{  
  delete m_inventory;
}

bool Player::init(Scene * scene)
{
  m_camera = scene->getCamera();
  m_camera->registerMovedCallback(&cameraMovedCallback);
  m_camera->registerRotatedCallback(&cameraRotatedCallback);

  m_inventory = new Inventory();

  Object * plant_harvest_obj = scene->addObject("game/models/energy_pull_tool.obj",glm::vec3(0,0,0), false);
  plant_harvest_obj->setScale(glm::vec3(0.2f,0.2f,0.2f));

  std::map<int, Material * > materials =  plant_harvest_obj->getModel()->getMaterials();
  for(std::map<int,Material*>::iterator it = materials.begin(); it != materials.end();it++)
  { 
    it->second->setEmmissive(1.0f);
  }    


  Item * plant_harvest_item = new Item(plant_harvest_obj,true, ITEM_HARVEST_TOOL);
  plant_harvest_item->setInactive();
  m_inventory->addItem(plant_harvest_item);

  /*Object * plant_tool_obj = scene->addObject("game/models/energy_tool.obj",glm::vec3(0,0,0), false);
  Item * plant_tool_item = new Item(plant_tool_obj,true, ITEM_PLANT_TOOL);
  plant_tool_item->setInactive();
  m_inventory->addItem(plant_tool_item);*/

  Object * plant_tool_obj = scene->addObject("game/models/flower_tool.obj",glm::vec3(0,0,0), false);
  plant_tool_obj->setScale(glm::vec3(0.2f,0.2f,0.2f));
  Item * plant_tool_item = new Item(plant_tool_obj,true, ITEM_PLANT_TOOL);
  plant_tool_item->setInactive();
  m_inventory->addItem(plant_tool_item);


  Object * attack_plant_tool_obj = scene->addObject("game/models/attack_flower_tool.obj",glm::vec3(0,0,0), false);
  attack_plant_tool_obj->setScale(glm::vec3(0.2f,0.2f,0.2f));
  Item * attack_plant_tool_item = new Item(attack_plant_tool_obj,true, ITEM_ATTACK_PLANT_TOOL);
  attack_plant_tool_item->setInactive();
  m_inventory->addItem(attack_plant_tool_item);

  Object * trap_plant_tool_obj = scene->addObject("game/models/grab_flower_tool.obj",glm::vec3(0,0,0), false);
  trap_plant_tool_obj->setScale(glm::vec3(0.2f,0.2f,0.2f));
  Item * trap_plant_tool_item = new Item(trap_plant_tool_obj,true, ITEM_TRAP_PLANT_TOOL);
  trap_plant_tool_item->setInactive();
  m_inventory->addItem(trap_plant_tool_item);

  m_player_object = scene->addObject("game/models/dark_energy_monster.obj",glm::vec3(0,0,0), false);
  m_player_object->setShadowRenderOnly();




  setupGUI();

 // m_inventory->selectItem(0);

  return true;
}


void Player::setupGUI()
{
  m_gui =  Engine::addGUI();

  m_lower_text = m_gui->addText();
  m_lower_text->setPosition(glm::vec2(40.0f,10.0f));
  m_lower_text->setScale(0.25f);
  m_lower_text->setColor(glm::vec3(255,255,255));
  m_lower_text->setText("Health");
}

void Player::update()
{
  float delta = Engine::getTimeDelta();
  m_inventory->update();

  m_player_object->setPosition( m_camera->getPosition() );

  if(m_jump_state != 0)
    m_jump_time += delta;

  if( (m_jump_state == 1) && (m_jump_time > JUMP_TIME ) )
  {
    m_jump_state = -1;
    m_jump_time = 0.0f;
  }
  if( (m_jump_state == -1) && (m_jump_time > JUMP_CD ) )
  {
    m_jump_state = 0;
    m_jump_time = 0.0f;
  }


  if( Engine::isKeyReleasedAndPressed(' '))
  {
    if( m_jump_state == 0 )
    {
      m_jump_state = 1;
    }  
  }

  if( Engine::isKeyReleasedAndPressed('W'))
    m_movement_keys_pressed++;
  if( Engine::isKeyReleasedAndPressed('A'))
    m_movement_keys_pressed++;
  if( Engine::isKeyReleasedAndPressed('S'))
    m_movement_keys_pressed++;
  if( Engine::isKeyReleasedAndPressed('D'))
    m_movement_keys_pressed++;
   
  if( Engine::isKeyPressedAndReleased('W') )  
    m_movement_keys_pressed--;
  if( Engine::isKeyPressedAndReleased('A') )  
    m_movement_keys_pressed--;
  if( Engine::isKeyPressedAndReleased('S') )  
    m_movement_keys_pressed--;
  if( Engine::isKeyPressedAndReleased('D') )  
    m_movement_keys_pressed--;

  if(m_movement_keys_pressed != 0)
  {
   // if( m_walking_sound_sound->getStatus() != sf::Sound::Status::Playing  )
   //   m_walking_sound_sound->play();
  }
  else
  {
   // if( m_walking_sound_sound->getStatus() != sf::Sound::Status::Stopped  )
    //  m_walking_sound_sound->stop();
  }
  
 


  if(m_jump_state == 1)
    m_camera->move(glm::vec3(0,10,0)*delta );

  std::stringstream ss;
  ss << "   Energy: "<<m_energy ;
  m_lower_text->setText(ss.str());
}

void Player::cameraMovedCallback()
{
  glm::vec3 r(m_camera->getYaw(),-m_camera->getPitch()-90.0f,0);
  glm::vec3 d = m_camera->getDirection();
  //d.z = 0.0f;
  // d.x = 0.0f;


  Item * item = m_inventory->getSelectedItem();
  if(item != nullptr)
  {
    item->setPosition(m_camera->getPosition()+m_camera->getDirection()*0.5f +m_camera->getRight()*0.3f -m_camera->getUp() *0.2f);
    item->setRotation(r);
  }
 // m_weapon->setPosition(m_camera->getPosition()+m_camera->getDirection() +m_camera->getRight()*0.4f -m_camera->getUp() *0.3f);

 
 // m_weapon->setRotation(r);
}

void Player::cameraRotatedCallback()
{
  glm::vec3 r(m_camera->getYaw(),-m_camera->getPitch()-90.0f,0);
  glm::vec3 d = m_camera->getDirection();

  Item * item = m_inventory->getSelectedItem();
  if(item != nullptr)
  {
    item->setPosition(m_camera->getPosition()+m_camera->getDirection()*0.5f +m_camera->getRight()*0.3f -m_camera->getUp() *0.2f);
    item->setRotation(r);
  }
}

Object * Player::getWeapon()
{

  Item * item = m_inventory->getSelectedItem();
  if(item != nullptr)
  {
    if(item->isWeapon())
      return item->getObject();
  }
  return nullptr;
}

glm::vec3 Player::getPosition()
{
	return m_camera->getPosition();
}



void Player::checkAction(Environment * environment)
{
  if(m_inventory->getSelectedItem() != nullptr)
    m_inventory->getSelectedItem()->doAction(environment);
}
void Player::checkSecondaryAction(Environment * environment)
{
  if(m_inventory->getSelectedItem() != nullptr)
    m_inventory->getSelectedItem()->doSecondaryAction(environment);
}

void Player::drainEnergy(float val, bool deadly)
{
  m_energy-=val;
  if(deadly && (m_energy <0.0f) )
  {
    Game::end();
  }
  if( m_energy < 0.0f)
    m_energy = 0.0f;

  float scale = 1.0f/100.0f *   m_energy ;
  m_player_object->setScale(glm::vec3(scale,scale,scale));
}

void Player::gainEngery(float val)
{
  SoundManager::addAndPlaySound("game/models/sounds/healspell1.wav");
  m_energy += val;
  
  float scale = 1.0f/100.0f *   m_energy ;
  m_player_object->setScale(glm::vec3(scale,scale,scale));
}

float Player::getEnergy()
{
  return m_energy;
}