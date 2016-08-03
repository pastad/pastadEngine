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

void Mobs::spawnRandom(Scene * scene, unsigned int amount)
{

  for(int x=0; x < amount;x++)
  {
    Mob * mob =  new Mob();
    mob->initialize(scene);

    float rad = 30.0f;

    std::srand(std::time(0));
    int angle = std::rand() %360;

    glm::vec2 r = glm::rotate(glm::vec2(1,0), angle/180.0f * 3.0f ) * rad;

    std::cout <<angle << " "<< r.x <<" , "<<r.y <<std::endl;

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

    if(dis<2.0f)
    {
      std::cout << "Mob desolved" <<std::endl;

      env->refreshTargets((*it) , this);

      Engine::getScene()->removeObject((*it)->getObject());
      delete *it;
      m_mobs.erase(it);
      player->drainEnergy(25.0f);



    }
    else
    {
      (*it)->update(player,delta);
      /*bool can_move = true;
      
      Object * obj =  player->getWeapon();
      if( obj != nullptr)
      {
        if( Engine::getPhysicSubsystem()->collisionObjectObject((*it)->getObject(), obj ))
        {
         // std::cout << "HIT" <<std::endl;
          Engine::getScene()->removeObject((*it)->getObject());
          delete *it;
          m_mobs.erase(it);
          can_move = false;
        }
      }
      if(can_move)
      {*/
       /* glm::vec3 dir = glm::normalize( player->getPosition() - (*it)->getObject()->getPosition() );
        dir.y =0.0f;

        glm::vec3 npos;
        glm::vec3 tnpos = (*it)->getObject()->getPosition() +dir*step;

        bool move_ok = true;

       /* for(std::vector<Mob*>::iterator it2 = m_mobs.begin(); it2 != m_mobs.end();it2++)
        {
          float d = glm::distance(tnpos,(*it2)->getObject()->getPosition());
         // std::cout <<d<<std::endl;
          if( d < 0.3f && ( (*it2) != (*it) ) )
            move_ok = false;
        }*/
/*
        if( Engine::getPhysicSubsystem() != nullptr && move_ok)
        {
          if(Engine::getPhysicSubsystem()->collisionMovement(Engine::getScene(), (*it)->getObject()->getPosition() , dir, step, 0.2f, 0.1f, &npos))
            (*it)->getObject()->setPosition(npos);
        } */      
      //}
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

void Mobs::removeMob(Mob * m)
{
  for(std::vector<Mob*>::iterator it = m_mobs.begin(); it != m_mobs.end();)
  {
    if( (*it) == m )
    {
      Engine::getScene()->removeObject((*it)->getObject());
      delete *it;
      m_mobs.erase(it);
      it = m_mobs.end();
    }
    else
      it++;
  }
}
std::vector<Mob *> Mobs::getMobs()
{
  return m_mobs;
}

void Mobs::spawnNextWave()
{
  spawnRandom(Engine::getScene(),1);
  std::cout << "spawn mob wave" <<std::endl;
}