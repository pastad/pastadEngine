#include "Environment.h"

#include "Plant.h"
#include "Engine.h"
#include "Scene.h"
#include "Player.h"
#include "Object.h"
#include "Mobs.h"
#include "Mob.h"
#include "EnergyShot.h"

#include <iostream>

#include <ctime>
#include <cstdlib>


#include <glm/gtx/rotate_vector.hpp>

#define ENERGY_REACTION_FAR 5
#define ENERGY_REACTION_CLOSE 0.1
#define ENERGY_AMOUNT 25
#define ENERGY_MAX_SPEED 2


Environment::Environment()
{  
}

Environment::~Environment()
{  
  for(std::vector<Plant*>::iterator it = m_plants.begin(); it != m_plants.end(); )
  {
    delete (*it);
    m_plants.erase(it);
  }
  for(std::vector<EnergyShot*>::iterator it = m_energy_shots.begin(); it != m_energy_shots.end(); )
  {
    delete (*it);
    m_energy_shots.erase(it);
  }
  for(std::vector<Object *>::iterator it = m_energy_remains.begin(); it != m_energy_remains.end(); )
  {
    delete (*it);
    m_energy_remains.erase(it);
  }
}

bool Environment::initialize(Scene * scene)
{ 
  std::srand(std::time(0)); //use current time as seed for random generator
   
  // test init
  for(int x=0; x<10;x++)
  {
    int random_variable = std::rand() % 360;
    int dist = std::rand() % 40 +2;
    float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
    float dist_f = dist +r;
    glm::vec3 d = glm::rotateY(glm::vec3(1,0,0), (float)random_variable) * (float) dist_f;
    Object * obj =  scene->addObjectInstanced("game/models/grass2.obj",d, false ); 

  }

  return true;
}

void Environment::addPlant(Scene * scene , unsigned int type, glm::vec3 pos)
{
  Plant * plant =  new Plant(scene,type,pos);
  if(plant != nullptr)
    m_plants.push_back(plant);
}


void Environment::addEnergyRemains(std::vector<Object *> objs)
{
  for(std::vector<Object *>::iterator it =objs.begin(); it != objs.end();it++)
  {
    m_energy_remains.push_back(*it);
  }
}

void Environment::checkAction(Player * player)
{
 
}



Plant * Environment::getPlant(Object * obj)
{
  for(std::vector<Plant*>::iterator it = m_plants.begin(); it != m_plants.end();it++ )
  {
    if( (*it)->getObject() == obj )
      return (*it);
  }
 // std::cout<<(m_plants[0])->getObject()<<" "<<obj  << m_plants.size()<<std::endl;
  return nullptr;
}


void Environment::update(float delta, float sun_strength, Player * player, Mobs * mobs)
{

  for(std::vector<Plant*>::iterator it = m_plants.begin(); it != m_plants.end();it++ )
  {
    (*it)->update(delta, sun_strength,mobs, this );    
  }
  for(std::vector<Object *>::iterator it = m_energy_remains.begin(); it != m_energy_remains.end(); )
  {
    Object * remain = (*it);
    remain->rotate(glm::vec3(1,1,1) );
    float dist = glm::distance(remain->getPosition() , player->getPosition());
    if(dist < ENERGY_REACTION_CLOSE)
    {
       remain->rotate(glm::vec3(1,1,1) );
      float amount = remain->getScale().x * ENERGY_AMOUNT;
      Engine::getScene()->removeObject(remain);
      delete remain;
      m_energy_remains.erase(it);
      player->gainEngery(amount );
    }  
    else
    {
      if( dist < ENERGY_REACTION_FAR)
      {
        glm::vec3 move_vec = glm::normalize( player->getPosition() - remain->getPosition());
        move_vec*= delta* ENERGY_MAX_SPEED;
        remain->move(move_vec);          
      }
      it++;
    }
  }


  bool clean_up = false;
  for(std::vector<EnergyShot*>::iterator it = m_energy_shots.begin(); it != m_energy_shots.end(); )
  {
    bool mob_killed;

    bool remove_shot =(*it)->update(delta, mobs,&mob_killed, this);

    if( (*it)->getTarget() != nullptr )
    {    
      if( mob_killed)
      {
        if(mobs->getMobs().size() <=1 )
        {
          clean_up = true; 
        }
        else
        {
          Mob * target = (*it)->getTarget();        
          mobs->removeMob(target, this);
        }       
      }
      if(remove_shot)
      {
        Engine::getScene()->removeObject((*it)->getObject());
        delete (*it)->getObject();      
        delete (*it);
        (*it) = nullptr;
        m_energy_shots.erase(it);
           std::cout << "erased shot | left:"<<m_energy_shots.size()<<std::endl;
      }
      else
        it++;
    }
    else
      it++;
  }

  
  if(clean_up)
  {
    for(std::vector<EnergyShot*>::iterator it = m_energy_shots.begin(); it != m_energy_shots.end(); )
    {
      Engine::getScene()->removeObject((*it)->getObject());
      delete (*it);
      (*it) = nullptr;
      m_energy_shots.erase(it);
    }
  }

}

void Environment::addEnergyShot(Scene * scene, Mob * target, glm::vec3 start_position  )
{
  EnergyShot * es= new EnergyShot();
  es->initialize(scene,target,start_position, 25.0f);
  m_energy_shots.push_back(es);
}

void Environment::refreshTargets(Mob * old, Mobs * mobs)
{
  std::cout << "refreshTargets" <<std::endl;
  for(std::vector<EnergyShot*>::iterator it = m_energy_shots.begin(); it != m_energy_shots.end();it++ )
  {  
    (*it)->changeTarget(old, mobs);     
  }
  std::cout << "refreshTargets done" <<std::endl;
}