#include "Game.h"

#include "Scene.h"
#include "Object.h"
#include "Camera.h"
#include "Engine.h"
#include "PhysicSubsystem.h"
#include "Wave.h"
#include "Water.h"


#include "Camera.h"

#include "Player.h"
#include "Mobs.h"

#include <iostream>



Game::Game()
{  
}

Game::~Game()
{  
  if(m_player)
    delete m_player;
  if(m_scene)
    delete m_scene;
  if(m_mobs)
    delete m_mobs;
}


bool Game::initialize()
{
  m_player = new Player();

  m_scene = new Scene();
  m_scene->load("island-scene.xml");

  m_scene->getCamera()->setRotationAllowed();
  m_scene->getCamera()->applyPhysicsDrop(1.7f);
  m_scene->getCamera()->dontAllowUpDownTranslation();
  m_scene->getCamera()->setSurroundingOffset(0.2f);


  Engine::setPostProcessing(PP_FXAA,true);
  Engine::setPostProcessing(PP_HDR,true);


  m_mobs = new Mobs();
  m_mobs->initialize(m_scene);

  Water * water_1  = m_scene->addWaterEffect(glm::vec3(0,-0.25,0),6);
  Wave * wave_1 = water_1->addWave(glm::vec2(1,1),1.0f,0.01,0.01);
  Wave * wave_2 = water_1->addWave(glm::vec2(0,1),1.0f,0.01,0.01);


  std::vector<Object *> objs = m_scene->getObjects("game/models/island.obj");
  objs[0]->setPosition(glm::vec3(0,0,0));
  objs[0]->applyPhysics();
  objs[0]->applyPhysicsStatic();



  //std::cout << objs.size()<<std::endl;

  if(! m_player->init(m_scene)  )
  {
    delete m_player;
    delete m_scene;
    delete m_mobs;
    return false;
  }
  Engine::setScene(m_scene, false);

  return true;
}

void Game::update()
{
  float delta = Engine::getTimeDelta();

  m_player->update();
  m_mobs->update(m_player);
}