#include "Plant.h"

#include "Object.h"
#include "Scene.h"
#include "Engine.h"
#include "Mobs.h"
#include "Mob.h"
#include "Environment.h"


#include <glm/gtx/vector_angle.hpp> 

#include <iostream>

#define TREE_GROWTH_SPEED 2
#define TREE_GROWTH_SCALE_MAX 0.25

#define FLOWER_GROWTH_SPEED 20
#define FLOWER_GROWTH_SCALE_MAX 1.0

#define ATTACK_FLOWER_GROWTH_SPEED 20
#define ATTACK_FLOWER_REGEN_SPEED 20
#define ATTACK_FLOWER_GROWTH_SCALE_MAX 1.0
#define ATTACK_FLOWER_COOLDOWN 1.0
#define ATTACK_FLOWER_RANGE  5
#define ATTACK_FLOWER_RANGE_SOFT  10
#define ATTACK_FLOWER_CAPACITY 100

#define ENERGY_GROWTH_SPEED 20

#define SHOOTING_COST 25

Plant::Plant(Scene * scene , unsigned int type, glm::vec3 pos)
{
  m_type= type;
  m_cooldown = -2.0f;
  m_growth =m_energy_growth= 0.0f;
  m_stored_energy = 25.0f;

  if( type == PLANT_TREE)
  {
    m_object = scene->addObject("game/models/tree1.obj",pos, false);
    //m_object->setRotation(glm::vec3(0.0f,90.0f,90.0f));
    m_object->setScale(glm::vec3(0.25f,0.25f,0.25f));
    m_object->applyPhysics();
    m_object->applyPhysicsStatic();
  }
  if( type == PLANT_ENERGY_FLOWER)
  {   
    m_object = scene->addObject("game/models/flower.obj",pos, false);
    //m_object->setRotation(glm::vec3(0.0f,90.0f,90.0f));
    m_object->setScale(glm::vec3(0.25f,0.25f,0.25f));
    m_object->applyPhysics();
    m_object->applyPhysicsStatic();
  }
  if( type == PLANT_ATTACK_FLOWER)
  {   
    m_object = scene->addObject("game/models/attack_flower.obj",pos, false);
    //m_object->setRotation(glm::vec3(0.0f,90.0f,90.0f));
    m_object->setScale(glm::vec3(0.25f,0.25f,0.25f));
    m_object->applyPhysics();
    m_object->applyPhysicsStatic();
  }
}

Plant::~Plant()
{
}

void Plant::update(float delta, float sun_strength, Mobs * mobs, Environment * env)
{
  //std::cout << m_cooldown<<std::endl;

  // check attack
  if(m_type ==PLANT_ATTACK_FLOWER)
  {
    if(m_cooldown > 0.0f)
    {
      float d = m_cooldown -delta;    
      if(d > 0.0f)    
        m_cooldown = d;    
      else    
        m_cooldown = 0.0f;    
    }
    if( m_cooldown == 0.0f )
    {
      if(m_stored_energy >=SHOOTING_COST)
        attackMobs(mobs, env);
      //else
      //  std::cout << "no energy left "<<std::endl;
    }
  }

  // restore energy
  if(m_stored_energy < ATTACK_FLOWER_CAPACITY)
  {
    m_stored_energy+= delta *sun_strength * ATTACK_FLOWER_REGEN_SPEED;
    if(m_stored_energy > ATTACK_FLOWER_CAPACITY)
      m_stored_energy = ATTACK_FLOWER_CAPACITY;
  }

  // growth of the plant
  if(m_growth>100.0f)
  {
    m_growth = 100.0f;

    Object * en = Engine::getScene()->addObject("game/models/energy_remain.obj",getObject()->getPosition() +glm::vec3(0,0.83+0.05,0), false);
    en->setScale(glm::vec3(0.0f,0.0f,0.0f));
    m_energy.push_back(en);
    m_cooldown = 0.0f;
  }
  else
  {   
    if(m_growth != 100.0f)
    {
      if( m_type == PLANT_TREE)
      {
        m_growth+= delta *sun_strength * TREE_GROWTH_SPEED;

        float scale = TREE_GROWTH_SCALE_MAX * (m_growth/100.0f);
        m_object->setScale(glm::vec3(scale,scale,scale));
      }
      if( m_type == PLANT_ENERGY_FLOWER)
      {
        m_growth+= delta *sun_strength * FLOWER_GROWTH_SPEED;

        float scale = FLOWER_GROWTH_SCALE_MAX * (m_growth/100.0f);
        m_object->setScale(glm::vec3(scale,scale,scale));
      }
      if( m_type == PLANT_ATTACK_FLOWER)
      {
        m_growth+= delta *sun_strength * ATTACK_FLOWER_GROWTH_SPEED;

        float scale = ATTACK_FLOWER_GROWTH_SCALE_MAX * (m_growth/100.0f);
        m_object->setScale(glm::vec3(scale,scale,scale));
      }  
    }
  }
  if(m_growth == 100.0f)
  {
    if( m_type == PLANT_ENERGY_FLOWER)
    {   
      float ngrowth =   m_energy_growth+ delta *sun_strength * ENERGY_GROWTH_SPEED;
      if( ! ( ( m_energy.size() >=3 ) && (ngrowth >=100.0f) ) )
      {
        m_energy_growth = ngrowth;

        if(m_energy_growth >= 100.0f)
        {
          m_energy_growth = 0.0f;

          std::vector<glm::vec3> g;
          g.push_back(glm::vec3(-0.1,0,-0.05));
          g.push_back(glm::vec3(0,0,-0.1));

          m_energy[m_energy.size()-1]->setScale(glm::vec3(1.0f,1.0f,1.0f));

          Object * en = Engine::getScene()->addObject("game/models/energy_remain.obj",getObject()->getPosition()+g[m_energy.size()-1] +glm::vec3(0,0.83+0.05,0), false);
          en->setScale(glm::vec3(0.0f,0.0f,0.0f));
          m_energy.push_back(en);
        }
        else
        {
          float scale = 1.0f * (m_energy_growth/100.0f);
          m_energy[m_energy.size()-1]->setScale(glm::vec3(scale,scale,scale));
        }
      }
  
    }
  }
}


void Plant::attackMobs(Mobs* mobs, Environment * env)
{   
  Mob * m = mobs->getClosestMobInRange(m_object->getPosition(), ATTACK_FLOWER_RANGE_SOFT);
  if(m != nullptr)
  {
    float dist = glm::distance( m->getObject()->getPosition(),m_object->getPosition() );

    glm::vec3 dir = glm::normalize(m->getObject()->getPosition() -m_object->getPosition());

    glm::vec2 xz = glm::vec2(dir.x,dir.z);    
    float y= glm::angle(xz, glm::vec2(1,0) );

    if(dir.z>0.0f)
      y*= -1.0f;

    getObject()->setRotation(glm::vec3(0,glm::degrees(y),0));

    if(dist <ATTACK_FLOWER_RANGE )
    {
      env->addEnergyShot(Engine::getScene(), m, m_object->getPosition() +glm::vec3(0,0.83f,0));
      std::cout << "shooooot" <<std::endl;
   
      m_cooldown = ATTACK_FLOWER_COOLDOWN;
      m_stored_energy-= SHOOTING_COST;
    }
  }

}

std::vector<Object *> Plant::harvest()
{
  std::vector<Object *> t = m_energy;
  bool continue_growth = false;

  if(t.size() >0)
  {
    if( m_energy_growth <100.0f)
    {
      continue_growth = true;
    }
  }

  m_energy.clear();

  if(! continue_growth)
  {
    m_energy_growth = 0.0f;

    Object * en = Engine::getScene()->addObject("game/models/energy_remain.obj",getObject()->getPosition() +glm::vec3(0,0.83+0.05,0), false);
    en->setScale(glm::vec3(0.0f,0.0f,0.0f));
    m_energy.push_back(en);
  }
  else
  {
    t[t.size()-1]->setPosition(getObject()->getPosition() +glm::vec3(0,0.83+0.05,0));
    m_energy.push_back(t[t.size()-1]);
    t.pop_back();
  }


  return t;
}

Object * Plant::getObject()
{
  return m_object;
}

unsigned int Plant::getType()
{
  return m_type;
}

float Plant::getGrowth()
{
  return m_growth;
}