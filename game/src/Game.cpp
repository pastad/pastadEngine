#include "Game.h"

#include "Scene.h"
#include "Engine.h"

#include "Player.h"
#include "Camera.h"


Game::Game()
{  
}

Game::~Game()
{  
  if(m_player)
    delete m_player;
  if(m_scene)
    delete m_scene;
}


bool Game::initialize()
{
  m_player = new Player();

  m_scene = new Scene();

  if(! m_player->init(m_scene->getCamera())  )
  {
    delete m_player;
    delete m_scene;
  }


  return true;
}

void Game::update()
{
  float delta = Engine::getTimeDelta();

  
}