#include "Player.h"

#include "Camera.h"
#include "Scene.h"
#include "Object.h"
#include "Inventory.h"
#include "Item.h"

#include "Engine.h"

#include "GUI.h"
#include "Text.h"

#include <iostream>
#include <sstream>

Camera * Player::m_camera;
Inventory * Player::m_inventory;

float Player::m_energy;

GUI * Player::m_gui;
Text * Player::m_lower_text;

Player::Player()
{  
  m_energy = 100.0f;
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

 /* Object * spear_obj = scene->addObject("game/models/spear.obj",glm::vec3(0,0,0), false);
  Item * spear_item = new Item(spear_obj,true);
  spear_item->setInactive();
  m_inventory->addItem(spear_item);

  Object * hatchet_obj = scene->addObject("game/models/hatchet.obj",glm::vec3(0,0,0), false);
  Item * hatchet_item = new Item(hatchet_obj,true);
  hatchet_item->setInactive();
  m_inventory->addItem(hatchet_item);*/

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

  m_lower_text = m_gui->getText();
  m_lower_text->setPosition(glm::vec2(40.0f,10.0f));
  m_lower_text->setScale(0.25f);
  m_lower_text->setColor(glm::vec3(255,255,255));
  m_lower_text->setText("Health");
}

void Player::update()
{
  m_inventory->update();


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