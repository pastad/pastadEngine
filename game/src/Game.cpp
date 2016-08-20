#include "Game.h"

#include "Scene.h"
#include "Object.h"
#include "Camera.h"
#include "Engine.h"
#include "PhysicSubsystem.h"
#include "Wave.h"
#include "Water.h"
#include "Light.h"
#include "Camera.h"
#include "Skybox.h"

#include "Player.h"
#include "Mobs.h"
#include "Environment.h"
#include "Plant.h"
#include "SoundManager.h"

#include <iostream>

#include <glm/gtx/rotate_vector.hpp>

#include <GLFW/glfw3.h>


#define DAY_LENGTH 60
#define DEG_PER_SEC 360.0f / DAY_LENGTH

#define SHADOW_TIME 2

float Game::m_end_time;

Game::Game()
{  
  m_game_time = 0.0f ;
  m_spawn_done = false;
  m_ended = false;
  m_end_time = -1.0f;
}

Game::~Game()
{  
  if(m_player)
    delete m_player;
 // if(m_scene)
  //  delete m_scene;
  if(m_mobs)
    delete m_mobs;
  if(m_environment)
    delete m_environment;

}


bool Game::initialize()
{
  m_player = new Player();

  // setup the base scene
  m_scene = new Scene();
  m_scene->load("island-scene.xml");

  m_skybox = m_scene->setSkybox("game/models/skybox1/sk1");

  m_scene->getCamera()->setRotationAllowed();
  m_scene->getCamera()->applyPhysicsDrop(1.7f);
  m_scene->getCamera()->dontAllowUpDownTranslation();
  m_scene->getCamera()->setSurroundingOffset(0.2f);
  m_scene->getCamera()->dontAllowUpDownMovement();

  // set the engine specs
  Engine::setPostProcessing(PP_FXAA,true);
  Engine::setPostProcessing(PP_HDR,true);
  Engine::setPostProcessing(PP_BLOOM,true);
  
//  Engine::setShadowTechnique(ST_STANDARD);

  // setup the mobs and the environment
  m_mobs = new Mobs();
  m_mobs->initialize(m_scene);

  m_environment = new Environment();
  if(!m_environment->initialize(m_scene))
  {
    delete m_player;
    delete m_scene;
    delete m_mobs;
    return false;
  }

  Water * water_1  = m_scene->addWaterEffect(glm::vec3(0,-0.25,0),6);
  Wave * wave_1 = water_1->addWave(glm::vec2(1,1),1.0f,0.01,0.01);
  Wave * wave_2 = water_1->addWave(glm::vec2(0,1),1.0f,0.01,0.01);

  // set the params for our ground
  std::vector<Object *> objs = m_scene->getObjects("game/models/island.obj");
  objs[0]->setPosition(glm::vec3(0,0,0));
  objs[0]->applyPhysics();
  objs[0]->applyPhysicsStatic();

  // setup the sun 
  m_sun = m_scene->getLights()[0];
  m_sun->setDirection(glm::vec3(0,-1.0,1.0f));
  glm::vec3 v = glm::rotateZ(m_sun->getDirection(), 45.0f) ;
  m_sun->setDirection(v);

  // and intitialize our player

  if(! m_player->init(m_scene)  )
  {
    delete m_player;
    delete m_scene;
    delete m_mobs;
    return false;
  }


  if (!m_background_sound_buffer.loadFromFile("game/models/sounds/wind1.wav"))
  {
	//return false;
  }
  else
  {
	  m_background_sound_sound.setBuffer(m_background_sound_buffer);
	  m_background_sound_sound.play();
	  m_background_sound_sound.setVolume(SOUND_BACKGROUND_VOLUME);
	  m_background_sound_sound.setLoop(true);
  }

  //m_scene->setFog(glm::vec3(1,1,1.0f), 0.000003f , 10.0f);

  glm::vec3 vd = glm::rotateZ(m_sun->getDirection(), glm::radians(-160.0f)  ) ;
  m_sun->setDirection(vd);

  // set the scene to the current one
  Engine::setScene(m_scene, false);

  return true;
}

void Game::update()
{
  float delta = Engine::getTimeDelta();

  if(Engine::isKeyReleasedAndPressed(GLFW_KEY_ESCAPE))
    end();

  // if end time >=0.0f we are at the end
  if( m_end_time < 0.0f)
  {
    // time update and reset if next day
    m_game_time += delta;
    if(m_game_time >= DAY_LENGTH)
    {
      m_spawn_done = false;
      m_game_time -= DAY_LENGTH;
    }

    // turn the sun and calculate a strength value for refreshing energy
    glm::vec3 v = glm::rotateZ(m_sun->getDirection(), glm::radians(DEG_PER_SEC*delta)  ) ;
    m_sun->setDirection(v);
    float sun_strength = -1.0f * m_sun->getDirection().y;
    if(sun_strength <0.0f)
      sun_strength = 0.0f;

    if( m_game_time > SHADOW_TIME)
    {
      m_scene->setFog(glm::vec3(sun_strength,sun_strength,sun_strength), 0.0001f , 0.0f);
      m_skybox->setLightStrength(sun_strength);
    }
    else
    {
      float fac = sun_strength + (SHADOW_TIME-m_game_time);
      m_scene->setFog(glm::vec3(fac, fac, fac), fac , 0.0f);
      m_skybox->setLightStrength(fac*fac);
    }


    // do the spawns here
    if( (m_game_time > DAY_LENGTH / 2.0f) && (!m_spawn_done) )
    {
      m_mobs->spawnNextWave();
      m_spawn_done = true;
    }
    
    // update the player, mobs and the environment
    m_player->update();
    m_mobs->update(m_player,m_environment);
    m_environment->update(delta, sun_strength,m_player,m_mobs );

    SoundManager::update();


    // checks mouse keys and triggers actions
    if(Engine::isMouseButtonReleasedAndPressed(0))
    {
      m_player->checkAction(m_environment);
    }
     if(Engine::isMouseButtonReleasedAndPressed(1))
    {
      m_player->checkSecondaryAction(m_environment);
    } 

    // debug triggers  ------------- DELETE ME AT END
     if(Engine::isKeyReleasedAndPressed('F'))
      m_mobs->spawnNextWave();
  }
  else
  {
    m_end_time += delta;

    if( m_end_time > SHADOW_TIME )
      m_ended = true;
    else
    {
    float sun_strength = -1.0f * m_sun->getDirection().y;
    if(sun_strength <0.0f)
      sun_strength = 0.0f;

      float fac = sun_strength + (m_end_time);
      m_scene->setFog(glm::vec3(fac, fac, fac), fac , 0.0f);
      m_skybox->setLightStrength(fac*fac);
    }
  }

}

bool Game::hasEnded()
{
  return m_ended;
}
void Game::end()
{
  m_end_time = 0;
}