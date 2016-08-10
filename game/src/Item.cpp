#include "Item.h"

#include "Object.h"
#include "Engine.h"
#include "Camera.h"
#include "Scene.h"
#include "PhysicSubsystem.h"
#include "Ray.h"

#include "Player.h"
#include "Plant.h"
#include "Environment.h"
#include "SoundManager.h"

#include <iostream>

Item::Item(Object * obj, bool is_weapon, unsigned int type): m_is_weapon(is_weapon),m_type(type)
{  
  m_object = obj;
}
Item::~Item()
{  
}

void Item::setPosition(glm::vec3 pos)
{
  m_object->setPosition(pos);
}

void Item::setRotation(glm::vec3 rot)
{
  m_object->setRotation(rot);
}

void Item::setActive()
{
  m_object->setVisible();
}

void Item::setInactive()
{
  m_object->setInvisible();
}

Object * Item::getObject()
{
  return m_object;
}
bool Item::isWeapon()
{
  return m_is_weapon;
}
unsigned int Item::getType()
{
  return m_type;
}

void Item::doAction(Environment * env)
{
  if(m_type == ITEM_PLANT_TOOL)
  {
    if( Player::getEnergy() >= 25.0f )
    {
      glm::vec3 npos;
      glm::vec3 dir = Engine::getScene()->getCamera()->getDirection() ;
      dir.y=0.0f;
      //if(Engine::getPhysicSubsystem()->collisionMovement( Engine::getScene() , Player::getPosition(), dir, 1.0f, 1.0f , 1.7f, &npos) )// float step, float surrounding_offset, float bottom_offset, glm::vec3 * new_pos)
      if(getPlacement(&npos))
      {
        //std::cout << npos.x << ", "<< npos.y << ", "<< npos.z << std::endl;
        env->addPlant(Engine::getScene(), PLANT_ENERGY_FLOWER, npos);

        Player::drainEnergy(25.0f,false);
        SoundManager::addAndPlaySound("game/models/sounds/rustle19.flac");

      }
    }
  }
  if( (m_type == ITEM_ATTACK_PLANT_TOOL) || (m_type == ITEM_TRAP_PLANT_TOOL)  )
  {
    if( Player::getEnergy() >= 15.0f )
    {
      glm::vec3 npos;
      glm::vec3 dir = Engine::getScene()->getCamera()->getDirection() ;
      dir.y=0.0f;
      if(getPlacement(&npos))
      {
        //std::cout << npos.x << ", "<< npos.y << ", "<< npos.z << std::endl;
        if(m_type == ITEM_ATTACK_PLANT_TOOL) 
          env->addPlant(Engine::getScene(), PLANT_ATTACK_FLOWER, npos);
        if(m_type == ITEM_TRAP_PLANT_TOOL) 
          env->addPlant(Engine::getScene(), PLANT_TRAP_FLOWER, npos);
        SoundManager::addAndPlaySound("game/models/sounds/rustle19.flac");
        Player::drainEnergy(15.0f,false);

      }
    }
  }
  if(m_type == ITEM_HARVEST_TOOL)
  {
    Object * obj = Engine::pickObjectAtCenter();
    Plant * plant = env->getPlant(obj);
    if(plant != nullptr)
    {
      if(plant->getType() ==PLANT_ENERGY_FLOWER)
      {
        std::vector<Object *> en = plant->harvest();
        env->addEnergyRemains(en);
      }
    }
  }
}


void Item::doSecondaryAction(Environment * env)
{
 
}

bool  Item::getPlacement( glm::vec3 *pos)
{
  Scene * scene = Engine::getScene();
  Camera * cam =  Engine::getScene()->getCamera();

  float range = 5.0f;

  if( (scene != nullptr) )
  {
    Ray r2( cam->getPosition() , cam->getDirection(), 0.0f, range);
    float distance2 =0.0f;
    bool res = Engine::getPhysicSubsystem()->collisionRayScene(scene, &r2, &distance2);

    if(res)
      std::cout <<distance2<<std::endl;

    if( res && (distance2 < range) )
    {
      *pos = cam->getPosition() +cam->getDirection()*distance2;
      std::cout << pos->x <<" ,"<< pos->y <<" ,"<< pos->z <<std::endl;
      return true;
    }
  }

  return false;

}