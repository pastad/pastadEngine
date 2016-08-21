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



float Game::m_game_time;
float Game::m_end_time;

unsigned int Game::m_sound_effect_volume = 30;
unsigned int Game::m_sound_background_volume = 10;

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
	Engine::setPostProcessing(PP_FXAA, true);
	Engine::setPostProcessing(PP_HDR, true);
	Engine::setPostProcessing(PP_BLOOM, true);

	//  Engine::setShadowTechnique(ST_STANDARD);

	// setup the mobs and the environment
	m_mobs = new Mobs();
	m_mobs->initialize(m_scene);

	m_environment = new Environment();
	if (!m_environment->initialize(m_scene))
	{
		delete m_player;
		delete m_scene;
		delete m_mobs;
		return false;
	}

	Water * water_1 = m_scene->addWaterEffect(glm::vec3(0, -0.25, 0), 6);
	Wave * wave_1 = water_1->addWave(glm::vec2(1, 1), 1.0f, 1.0f, 0.01f);
	Wave * wave_2 = water_1->addWave(glm::vec2(0, 1), 1.0f, 1.0f, 0.01f);

	// set the params for our ground
	std::vector<Object *> objs = m_scene->getObjects("game/models/island.obj");
	objs[0]->setPosition(glm::vec3(0, 0, 0));
	objs[0]->applyPhysics();
	objs[0]->applyPhysicsStatic();

	// setup the sun 
	m_sun = m_scene->getLights()[0];
	m_sun->setDirection(glm::vec3(0, -1.0, 1.0f));
	glm::vec3 v = glm::rotateZ(m_sun->getDirection(), 45.0f);
	m_sun->setDirection(v);

	// and intitialize our player

	if (!m_player->init(m_scene))
	{
		delete m_player;
		delete m_scene;
		delete m_mobs;
		return false;
	}
	//m_scene->setFog(glm::vec3(1,1,1.0f), 0.000003f , 10.0f);

	glm::vec3 vd = glm::rotateZ(m_sun->getDirection(), glm::radians(-160.0f));
	m_sun->setDirection(vd);

	m_game_day = 0;
	m_game_time = 0.0f;


  if (!m_background_sound_buffer.loadFromFile("game/sounds/wind1.wav"))
  {
   // return false;
  }

  m_background_sound_sound.setBuffer(m_background_sound_buffer);
  m_background_sound_sound.play();
  m_background_sound_sound.setVolume(Game::getSoundEffectVolume() *0.3f);
  m_background_sound_sound.setLoop(true);


	// set the scene to the current one
	Engine::setScene(m_scene, false);

	loadConfig("game/game_config.xml");
	loadHighscore();

	return true;
}

void Game::update()
{
	float delta = Engine::getTimeDelta();
	if (m_first_run || delta >0.5f) // for the fade in halt
	{
		m_first_run = false;
		delta = 0.0f;
	}

	if (Engine::isKeyReleasedAndPressed(GLFW_KEY_ESCAPE))
		end();

	// if end time >=0.0f we are at the end
	if (m_end_time < 0.0f)
	{
		// time update and reset if next day
		m_game_time += delta;
		if (m_game_time >= DAY_LENGTH)
		{
			m_spawn_done = false;
			m_game_time -= DAY_LENGTH;
			m_game_day++;
		}

		// turn the sun and calculate a strength value for refreshing energy
		glm::vec3 v = glm::rotateZ(m_sun->getDirection(), glm::radians(DEG_PER_SEC*delta));
		m_sun->setDirection(v);
		float sun_strength = -1.0f * m_sun->getDirection().y;
		if (sun_strength <0.0f)
			sun_strength = 0.0f;

		if ((m_game_time > SHADOW_TIME) || (m_game_day > 0))
		{
			m_scene->setFog(glm::vec3(sun_strength, sun_strength, sun_strength), 0.0001f, 0.0f);
			m_skybox->setLightStrength(sun_strength);
		}
		else
		{
			//std::cout << "fade in" <<std::endl;
			float fac = sun_strength + (SHADOW_TIME - m_game_time);
			m_scene->setFog(glm::vec3(fac, fac, fac), fac, 0.0f);
			m_skybox->setLightStrength(fac*fac);
		}


		// do the spawns here
		if ((m_game_time > DAY_LENGTH / 2.0f) && (!m_spawn_done))
		{
			m_mobs->spawnNextWave();
			m_spawn_done = true;
		}

		// update the player, mobs and the environment
		m_player->update();
		m_mobs->update(m_player, m_environment);
		m_environment->update(delta, sun_strength, m_player, m_mobs);

		SoundManager::update();


		// checks mouse keys and triggers actions
		if (Engine::isMouseButtonReleasedAndPressed(0))
		{
			m_player->checkAction(m_environment);
		}
		if (Engine::isMouseButtonReleasedAndPressed(1))
		{
			m_player->checkSecondaryAction(m_environment);
		}

		// debug triggers  ------------- DELETE ME AT END
		if (Engine::isKeyReleasedAndPressed('F'))
			m_mobs->spawnNextWave();
	}
	else
	{
		m_end_time += delta;

		//std::cout << "END"<<std::endl;

		if (m_end_time > SHADOW_TIME)
    {
			m_ended = true;
      Engine::stopRunning();
    }
		else
		{
			float sun_strength = -1.0f * m_sun->getDirection().y;
			if (sun_strength <0.0f)
				sun_strength = 0.0f;

			float fac = sun_strength + m_end_time;

			m_scene->setFog(glm::vec3(fac, fac, fac), fac, 0.0f);
			m_skybox->setLightStrength(fac*fac*fac);
		}
	}

}
bool Game::hasEnded()
{
  if (m_ended)
  {
    m_last_highscore.days = m_game_day;
    m_last_highscore.time = m_game_time;
    m_last_highscore.energy_left = m_player->getEnergy();

    if (m_last_highscore.days >= m_best_highscore.days)
    {
      if (m_last_highscore.days > m_best_highscore.days)
      {
        m_best_highscore.days = m_last_highscore.days;
        m_best_highscore.time = m_last_highscore.time;
        m_best_highscore.energy_left = m_last_highscore.energy_left;
      }
      else
      {
        if (m_last_highscore.time >= m_best_highscore.time)
        {
          if (m_last_highscore.time > m_best_highscore.time)
          {
            m_best_highscore.days = m_last_highscore.days;
            m_best_highscore.time = m_last_highscore.time;
            m_best_highscore.energy_left = m_last_highscore.energy_left;
          }
          else
          {
            if (m_last_highscore.energy_left > m_best_highscore.energy_left)
            {
              m_best_highscore.days = m_last_highscore.days;
              m_best_highscore.time = m_last_highscore.time;
              m_best_highscore.energy_left = m_last_highscore.energy_left;
            }
          }
        }
      }
    }

    saveHighscore();
  }
  return m_ended;
}
void Game::end()
{
	m_end_time = 0;
}
bool Game::saveConfig(std::string path, unsigned int effect_volume, unsigned int background_volume)
{
	tinyxml2::XMLDocument document;
	tinyxml2::XMLNode * root = document.NewElement("GameConfig");
	document.InsertFirstChild(root);

	tinyxml2::XMLElement * element_sev = document.NewElement("SoundEffectVolume");
	root->InsertEndChild(element_sev);
	element_sev->SetAttribute("value", effect_volume);

	tinyxml2::XMLElement * element_sbv = document.NewElement("SoundBackgroundVolume");
	root->InsertEndChild(element_sbv);
	element_sbv->SetAttribute("value", background_volume);


	tinyxml2::XMLError eResult = document.SaveFile(path.c_str());


	return true;
}

bool Game::loadConfig(std::string path)
{
	return readConfig(path, &m_sound_effect_volume, &m_sound_background_volume);
}
bool Game::readConfig(std::string path, unsigned int * sev, unsigned int * sbv)
{
	tinyxml2::XMLDocument document;
	tinyxml2::XMLError result = document.LoadFile(path.c_str());

	if (result != tinyxml2::XML_SUCCESS)
	{
		return false;
	}

	tinyxml2::XMLNode * root = document.FirstChild();

	if (root != nullptr)
	{
		tinyxml2::XMLElement * child = root->FirstChildElement();

		while (child != nullptr)
		{
			std::string type = std::string(child->Name());

			if (type == "SoundEffectVolume")
			{
				child->QueryUnsignedAttribute("value", sev);
			}
			if (type == "SoundBackgroundVolume")
			{
				child->QueryUnsignedAttribute("value", sbv);
			}


			child = child->NextSiblingElement();
		}
	}
	return true;
}

unsigned int Game::getSoundEffectVolume()
{
	return m_sound_effect_volume;
}
unsigned int Game::getSoundBackgroundVolume()
{
	return m_sound_background_volume;
}
float Game::getGameTime()
{
	return m_game_time;
}

bool Game::readHighsore(std::string path, HighScore *last, HighScore * best)
{
	tinyxml2::XMLDocument document;
	tinyxml2::XMLError result = document.LoadFile(path.c_str());

	if (result != tinyxml2::XML_SUCCESS)
	{
		return false;
	}

	tinyxml2::XMLNode * root = document.FirstChild();

	last->days = 0;
	last->time = 0;
	last->energy_left = 0;

	best->days = 0;
	best->time = 0;
	best->energy_left = 0;

	if (root != nullptr)
	{
		tinyxml2::XMLElement * child = root->FirstChildElement();

		while (child != nullptr)
		{
			std::string type = std::string(child->Name());

			if (type == "LastGame")
			{
				child->QueryUnsignedAttribute("days", &last->days);
				child->QueryFloatAttribute("time", &last->time);
				child->QueryFloatAttribute("energy_left", &last->energy_left);
			}
			if (type == "BestGame")
			{
				child->QueryUnsignedAttribute("days", &best->days);
				child->QueryFloatAttribute("time", &best->time);
				child->QueryFloatAttribute("energy_left", &best->energy_left);
			}


			child = child->NextSiblingElement();
		}
	}
	return true;
}

void Game::loadHighscore()
{
	readHighsore("game/game_highscore.xml", &m_last_highscore, &m_best_highscore);
}

void Game::saveHighscore()
{
	tinyxml2::XMLDocument document;
	tinyxml2::XMLNode * root = document.NewElement("HighScore");
	document.InsertFirstChild(root);

	tinyxml2::XMLElement * element_last = document.NewElement("LastGame");
	root->InsertEndChild(element_last);
	element_last->SetAttribute("days", m_last_highscore.days);
	element_last->SetAttribute("time", m_last_highscore.time);
	element_last->SetAttribute("energy_left", m_last_highscore.energy_left);

	tinyxml2::XMLElement * element_best = document.NewElement("BestGame");
	root->InsertEndChild(element_best);
	element_best->SetAttribute("days", m_best_highscore.days);
	element_best->SetAttribute("time", m_best_highscore.time);
	element_best->SetAttribute("energy_left", m_best_highscore.energy_left);


	tinyxml2::XMLError eResult = document.SaveFile("game/game_highscore.xml");

}