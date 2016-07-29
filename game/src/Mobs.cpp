#include "Mobs.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>


#include "Mob.h"
#include "Scene.h"
#include "Engine.h"
#include "PhysicSubsystem.h"
#include "Player.h"
#include "Object.h"


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

void Mobs::update(Player * player)
{
  float delta = Engine::getTimeDelta();
  float step = delta *5.0f;

  Object * obj =  player->getWeapon();
  if( obj != nullptr)
  {
    for(std::vector<Mob*>::iterator it = m_mobs.begin(); it != m_mobs.end();)
    {

      if( Engine::getPhysicSubsystem()->collisionObjectObject((*it)->getObject(), obj ))
      {
        std::cout << "HIT" <<std::endl;
        Engine::getScene()->removeObject((*it)->getObject());
        delete *it;
        m_mobs.erase(it);
      }
      else
      {
        glm::vec3 dir = glm::normalize( player->getPosition() - (*it)->getObject()->getPosition() );
        dir.y =0.0f;

        glm::vec3 npos;
        glm::vec3 tnpos = (*it)->getObject()->getPosition() +dir*step;

        bool move_ok = true;

        for(std::vector<Mob*>::iterator it2 = m_mobs.begin(); it2 != m_mobs.end();it2++)
        {
          float d = glm::distance(tnpos,(*it2)->getObject()->getPosition());
         // std::cout <<d<<std::endl;
          if( d < 0.3f && ( (*it2) != (*it) ) )
            move_ok = false;
        }

        if( Engine::getPhysicSubsystem() != nullptr && move_ok)
        {
          if(Engine::getPhysicSubsystem()->collisionMovement(Engine::getScene(), (*it)->getObject()->getPosition() , dir, step, 0.2f, 0.1f, &npos))
            (*it)->getObject()->setPosition(npos);
        }
        it++;
      }


    }
  }
 
}
