#include "Mobs.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <glm/gtx/rotate_vector.hpp>

#include "Mob.h"
#include "Scene.h"
#include "Engine.h"
#include "PhysicSubsystem.h"
#include "Player.h"
#include "Object.h"

#include "Environment.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

Mobs::Mobs()
{  
  m_drain = 0.0f;
  m_wave = 0;
}

Mobs::~Mobs()
{  
  for(int x=0; x< m_mobs.size();x++)
    delete m_mobs[x];
}

bool Mobs::initialize(Scene * scene)
{
	//Mob * m = new Mob();
  //m->initialize(scene);

  //m_mobs.push_back(m);

  /*for(int x= 0; x< 5;x++)
  {
    m = new Mob();
    m->initialize(scene);
    m->setPosition(glm::vec3(x*0.5f+5,0.5f,10.0f));

    m_mobs.push_back(m);
  }*/
}

void Mobs::spawnRandom(Scene * scene, unsigned int min_amount, unsigned int max_amount, float drain)
{

  std::srand(std::time(0));
  int amount = std::rand()  %( max_amount - min_amount)    + min_amount;
  float damage =   drain /  ((float )amount);

  std::cout << "hpm " <<damage<<std::endl;
  for(int x=0; x < amount;x++)
  {
    Mob * mob =  new Mob();
    if( !mob->initialize(scene, damage))
    {
        std::cout << "ERRRRRORRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR --------------- ERRRRRORRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR"<<std::endl;
    }

    float rad = 50.0f;

    int angle = std::rand() %360;

    glm::vec2 r = glm::rotate(glm::vec2(1,0), angle/180.0f * 3.0f ) * rad;

    //std::cout <<angle << " "<< r.x <<" , "<<r.y <<std::endl;

    mob->setPosition(glm::vec3(r.x,0.7f,r.y));

    m_mobs.push_back(mob);
  }
}

void Mobs::update(Player * player, Environment * env)
{
  float delta = Engine::getTimeDelta();
  float step = delta *5.0f;

  for(std::vector<Mob*>::iterator it = m_mobs.begin(); it != m_mobs.end();)
  {

    float dis =  glm::distance((*it)->getObject()->getPosition(), player->getPosition());

    if( dis < 1.5f )
    {
      std::cout << "Mob desolved" <<std::endl;     

      player->drainEnergy((*it)->getHealth());

      removeMob((*it), env);
    }
    else
    {
      (*it)->update(player,delta); 
      it++;
    }

  }

}



Mob * Mobs::getClosestMobInRange(glm::vec3 pos , float range)
{
  Mob * ret  =nullptr;
  float cr =  -1.0f;

  for(std::vector<Mob*>::iterator it = m_mobs.begin(); it != m_mobs.end();it++)
  {
    float  distance = glm::distance( (*it)->getObject()->getPosition(),pos);
    if(distance <range)
    {
      if(cr == -1.0f)
      {
        cr = distance;
        ret = (*it);
      }
      else
      {
        if(distance <cr )
        {
          cr = distance;
          ret = (*it);
        }
      }
    }
  }
  return ret;
}

void Mobs::removeMob(Mob * m, Environment * env)
{
  Mob * target = m;
 
  for(std::vector<Mob*>::iterator it = m_mobs.begin(); it != m_mobs.end();)
  {
    if( (*it) == m )
    {
      std::cout << "removed mob| left:"<<m_mobs.size()<<std::endl;
      Engine::getScene()->removeObject((*it)->getObject());
      delete (*it)->getObject();
      delete *it;
      (*it ) = nullptr;
      m_mobs.erase(it);
      it = m_mobs.end();
    }
    else
      it++;
  }
  env->refreshTargets(target , this);
}
std::vector<Mob *> Mobs::getMobs()
{
  return m_mobs;
}

void Mobs::spawnNextWave()
{
  m_wave++;
  m_drain +=450.0f;
  if(m_wave < 13)
    spawnRandom(Engine::getScene(),8,10,m_drain);
  else
  {
    if(m_wave < 26)
      spawnRandom(Engine::getScene(),16,20,m_drain);
    else
       spawnRandom(Engine::getScene(),32,40,m_drain);
  }
  std::cout << "spawn mob wave "<<m_wave <<std::endl;
}