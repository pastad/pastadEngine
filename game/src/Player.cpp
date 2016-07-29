#include "Player.h"

#include "Camera.h"
#include "Scene.h"
#include "Object.h"
#include "Inventory.h"
#include "Item.h"
#include "GUI.h"
#include "Engine.h"


#include <iostream>

Camera * Player::m_camera;
Inventory * Player::m_inventory;

float Player::m_health;
float Player::m_hunger;

GUI * Player::m_gui;

Player::Player()
{  
  m_health = 100.0f;
  m_hunger = 0.0f;
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

  Object * spear_obj = scene->addObject("game/models/spear.obj",glm::vec3(0,0,0), false);
  Item * spear_item = new Item(spear_obj,true);
  spear_item->setInactive();
  m_inventory->addItem(spear_item);

  Object * hatchet_obj = scene->addObject("game/models/hatchet.obj",glm::vec3(0,0,0), false);
  Item * hatchet_item = new Item(hatchet_obj,true);
  hatchet_item->setInactive();
  m_inventory->addItem(hatchet_item);

  m_gui =  Engine::addGUI();

 // m_inventory->selectItem(0);

  return true;
}

void Player::update()
{
  m_inventory->update();

  m_hunger+= 0.01f;
  if(m_hunger > 100.0f)
    m_hunger = 100.0f;

  if(m_health >100.0f)
    m_health = 100.0f;
  if(m_health <0.0f)
    m_health = 0.0f;
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