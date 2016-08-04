#include "Player.h"

#include "Camera.h"
#include "Scene.h"
#include "Object.h"
#include "Inventory.h"
#include "Item.h"

#include "Engine.h"

#include "GUI.h"
#include "Text.h"

#define JUMP_TIME 0.1f
#define JUMP_CD 1.0f

#include <iostream>
#include <sstream>

Camera * Player::m_camera;
Inventory * Player::m_inventory;

float Player::m_energy;

GUI * Player::m_gui;
Text * Player::m_lower_text;

int Player::m_jump_state;
float Player::m_jump_time;

Player::Player()
{  
  m_energy = 100.0f;
  m_jump_time =0.0f;
  m_jump_state = 0;
}

Player::~Player()
{  
  delete m_inventory;
}

bool Player::init(Scene * scene)
{
  m_camera = scene->getCamera();
  m_camera->registerMovedCallback(&cameraMovedCallback);

  m_inventory = new Inventory();

  Object * plant_harvest_obj = scene->addObject("game/models/hand.obj",glm::vec3(0,0,0), false);
  Item * plant_harvest_item = new Item(plant_harvest_obj,true, ITEM_HARVEST_TOOL);
  plant_harvest_item->setInactive();
  m_inventory->addItem(plant_harvest_item);

  Object * plant_tool_obj = scene->addObject("game/models/energy_tool.obj",glm::vec3(0,0,0), false);
  Item * plant_tool_item = new Item(plant_tool_obj,true, ITEM_PLANT_TOOL);
  plant_tool_item->setInactive();
  m_inventory->addItem(plant_tool_item);


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
    item->setPosition(m_camera->getPosition()+m_camera->getDirection() +m_camera->getRight()*0.4f -m_camera->getUp() *0.3f);
    item->setRotation(r);
  }
 // m_weapon->setPosition(m_camera->getPosition()+m_camera->getDirection() +m_camera->getRight()*0.4f -m_camera->getUp() *0.3f);

 
 // m_weapon->setRotation(r);
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

void Player::gainEngery(float val)
{
  m_energy += val;
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

void Player::drainEnergy(float val)
{
  m_energy-=val;
  if(m_energy<0.0f)
    m_energy = 0.0f;
  
}

float Player::getEnergy()
{
  return m_energy;
}