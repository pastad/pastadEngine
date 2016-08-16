#include "Player.h"

#include "Camera.h"
#include "Scene.h"
#include "Object.h"
#include "Inventory.h"
#include "Item.h"
#include "SoundManager.h"
#include "Game.h"
#include "EnergySpark.h"
#include "Ray.h"
#include "PhysicSubsystem.h"
#include "IOSubsystem.h"


#include "Engine.h"

#include "GUI.h"
#include "Text.h"
#include "Image.h"
#include "Material.h"
#include "Model.h"
#include "Light.h"




#include "Script.h"
#include "RotationScriptElement.h"

#define JUMP_TIME 0.1f
#define JUMP_CD 1.0f

#define SCALE_SPEED 1.5f

#include <iostream>
#include <sstream>

Camera * Player::m_camera;
Inventory * Player::m_inventory;

Object * Player::m_player_object;
Light * Player::m_player_light;

float Player::m_energy;

GUI * Player::m_gui;
Text * Player::m_lower_text;
Image * Player::m_crosshair_active;
Image * Player::m_crosshair_inactive;

int Player::m_jump_state;
float Player::m_jump_time;

unsigned int Player::m_movement_keys_pressed;

sf::SoundBuffer * Player::m_walking_sound_buffer;
sf::Sound * Player::m_walking_sound_sound;

std::vector< Object *> Player::m_available_sparks;

unsigned int Player::m_available_sparks_animation[AMOUNT_SPARKS];


Player::Player()
{  
  m_energy = 100.0f;
  m_jump_time = 0.0f;
  m_jump_state = 0;
  m_movement_keys_pressed = 0;
}

Player::~Player()
{  
  delete m_inventory;
  delete m_walking_sound_sound;
  delete m_walking_sound_buffer;

  for( std::vector<Object *>::iterator it = m_available_sparks.begin();  it != m_available_sparks.end() ; )
  {
    m_available_sparks.erase(it);
  }
}

bool Player::init(Scene * scene)
{
  m_camera = scene->getCamera();
  m_camera->registerMovedCallback(&cameraMovedCallback);
  m_camera->registerRotatedCallback(&cameraRotatedCallback);
  IOSubsystem::registerMouseScrollCallback(&mouseScrollCallback);

  m_inventory = new Inventory();

  //Object * plant_harvest_obj = scene->addObject("game/models/energy_pull_tool.obj",glm::vec3(0,0,0), false);
 // plant_harvest_obj->setScale(glm::vec3(0.2f,0.2f,0.2f));


  //std::map<int, Material * > materials =  plant_harvest_obj->getModel()->getMaterials();
  //for(std::map<int,Material*>::iterator it = materials.begin(); it != materials.end();it++)
 // { 
 //   it->second->setEmmissive(1.0f);
 // }    

  //for(int x=0; x< AMOUNT_SPARKS; x++)
  //  m_available_sparks_animation[x] = 0;


  //Item * plant_harvest_item = new Item(plant_harvest_obj,true, ITEM_HARVEST_TOOL);
  //plant_harvest_item->setInactive();
 // m_inventory->addItem(plant_harvest_item);

  /*Object * plant_tool_obj = scene->addObject("game/models/energy_tool.obj",glm::vec3(0,0,0), false);
  Item * plant_tool_item = new Item(plant_tool_obj,true, ITEM_PLANT_TOOL);
  plant_tool_item->setInactive();
  m_inventory->addItem(plant_tool_item);*/

  Object * plant_tool_obj = scene->addObject("game/models/flower.obj",glm::vec3(0,0,0), false);
  plant_tool_obj->setScale(glm::vec3(0.2f,0.2f,0.2f));
  Item * plant_tool_item = new Item(plant_tool_obj,true, ITEM_PLANT_TOOL);
  plant_tool_item->setInactive();
  m_inventory->addItem(plant_tool_item);


  Object * attack_plant_tool_obj = scene->addObject("game/models/attack_flower.obj",glm::vec3(0,0,0), false);
  attack_plant_tool_obj->setScale(glm::vec3(0.2f,0.2f,0.2f));
  Item * attack_plant_tool_item = new Item(attack_plant_tool_obj,true, ITEM_ATTACK_PLANT_TOOL);
  attack_plant_tool_item->setInactive();
  m_inventory->addItem(attack_plant_tool_item);

  Object * trap_plant_tool_obj = scene->addObject("game/models/grab_flower.obj",glm::vec3(0,0,0), false);
  trap_plant_tool_obj->setScale(glm::vec3(0.2f,0.2f,0.2f));
  Item * trap_plant_tool_item = new Item(trap_plant_tool_obj,true, ITEM_TRAP_PLANT_TOOL);
  trap_plant_tool_item->setInactive();
  m_inventory->addItem(trap_plant_tool_item);



  m_player_object = scene->addObject("game/models/dark_energy_monster.obj",glm::vec3(0,0,0), false);
  m_player_object->setShadowRenderOnly();

  m_player_light =  scene->addLight();
  m_player_light->setPoint(glm::vec3(2,4,0),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),0.1f,0.2f,0.09f,0.062f,false);


  m_walking_sound_buffer = new sf::SoundBuffer();
  m_walking_sound_sound = new sf::Sound();

  if (!m_walking_sound_buffer->loadFromFile("game/sounds/wind1.wav"))
  {
    delete m_walking_sound_sound;
    delete m_walking_sound_buffer;
    return false;
  }

  m_walking_sound_sound->setBuffer(*m_walking_sound_buffer);
  m_walking_sound_sound->setLoop(true);
  m_walking_sound_sound->setVolume(Game::getSoundEffectVolume());




  for(int x=0; x < AMOUNT_SPARKS; x++)
  {
    Object *  spark = scene->addObject("game/models/energy_remain.obj",getPosition(), false);
    m_available_sparks.push_back(spark);
    Script * script = spark->addScript();
    RotationScriptElement * rse = script->addRotationScript();
    rse->setupAlways();
    rse->setup(glm::vec3(10000.0f,10000.0f,10000.0f));

    std::map<int, Material * > materials =  spark->getModel()->getMaterials();
    for(std::map<int,Material*>::iterator it = materials.begin(); it != materials.end();it++)
    { 
      it->second->setEmmissive(1.0f);
    }    
  }


  m_inventory->selectItem(0);

  setupGUI();

  return true;
}


void Player::setupGUI()
{
  m_gui =  Engine::addGUI();

  m_lower_text = m_gui->addText();
  m_lower_text->setPosition(glm::vec2(40.0f,10.0f));
  m_lower_text->setScale(0.25f);
  m_lower_text->setColor(glm::vec3(255,255,255));
  m_lower_text->setText("Health");

  m_crosshair_active = m_gui->addImage();
  m_crosshair_active->load("game/graphics/crosshair.png");
  m_crosshair_active->setScale(glm::vec2(0.25f, 0.25f));
  m_crosshair_active->setPosition(glm::vec2( ( Engine::getWindowWidth() ) /2.0f -  m_crosshair_active->getSize().x/2.0f , ( Engine::getWindowHeight()  ) / 2.0f - m_crosshair_active->getSize().y/2.0f ) );

  m_crosshair_inactive = m_gui->addImage();
  m_crosshair_inactive->load("game/graphics/crosshair_inactive.png");
  m_crosshair_inactive->setScale(glm::vec2(0.25f, 0.25f));
  m_crosshair_inactive->setPosition(glm::vec2( ( Engine::getWindowWidth() ) /2.0f -  m_crosshair_inactive->getSize().x/2.0f , ( Engine::getWindowHeight()  ) / 2.0f - m_crosshair_inactive->getSize().y/2.0f ) );
  m_crosshair_inactive->setInactive();
}

void Player::update()
{
  float delta = Engine::getTimeDelta();
  m_inventory->update();



  if(m_jump_state != 0)
    m_jump_time += delta;

  if( (m_jump_state == 1) && (m_jump_time > JUMP_TIME ) )
  {
    m_jump_state = -1;
    m_jump_time = 0.0f;
  }
  if( (m_jump_state == -1) && (m_jump_time > JUMP_CD ) )
  {
    m_jump_state = 0;
    m_jump_time = 0.0f;
  }


  if( Engine::isKeyReleasedAndPressed(' '))
  {
    if( m_jump_state == 0 )
    {
      m_jump_state = 1;
    }  
  }

  if( Engine::isKeyReleasedAndPressed('W'))
    m_movement_keys_pressed++;
  if( Engine::isKeyReleasedAndPressed('A'))
    m_movement_keys_pressed++;
  if( Engine::isKeyReleasedAndPressed('S'))
    m_movement_keys_pressed++;
  if( Engine::isKeyReleasedAndPressed('D'))
    m_movement_keys_pressed++;
   
  if( Engine::isKeyPressedAndReleased('W') )  
    m_movement_keys_pressed--;
  if( Engine::isKeyPressedAndReleased('A') )  
    m_movement_keys_pressed--;
  if( Engine::isKeyPressedAndReleased('S') )  
    m_movement_keys_pressed--;
  if( Engine::isKeyPressedAndReleased('D') )  
    m_movement_keys_pressed--;

  if(m_movement_keys_pressed != 0)
  {
   // if( m_walking_sound_sound->getStatus() != sf::Sound::Status::Playing  )
   //   m_walking_sound_sound->play();
  }
  else
  {
   // if( m_walking_sound_sound->getStatus() != sf::Sound::Status::Stopped  )
    //  m_walking_sound_sound->stop();
  }
  m_player_light->setPosition(m_camera->getPosition());

  unsigned int c =0;
  for( std::vector<Object *>::iterator it = m_available_sparks.begin(); it != m_available_sparks.end(); it++ )
  {
    if( m_available_sparks_animation[c] != 0 )
    {
      if( m_available_sparks_animation[c] == 1 ) // up 
      {
        if( (*it)->getScale().x < 1.0f )
        {
          float step = SCALE_SPEED *delta;
          (*it)->setScale( (*it)->getScale() + glm::vec3(step,step,step)  );
        }
        else
        {
          m_available_sparks_animation[c] = 0;
          (*it)->setScale( glm::vec3(1,1,1));
        }
      }
      if( m_available_sparks_animation[c] == 2 ) // down
      {
        if( (*it)->getScale().x > 0.0f )
        {
          float step = SCALE_SPEED *delta;
          (*it)->setScale( (*it)->getScale() - glm::vec3(step,step,step)  );
        }
        else
        {
          m_available_sparks_animation[c] = 0;
          (*it)->setInvisible();
          (*it)->setScale( glm::vec3(0,0,0));
        }
      }
    }
    c++;
  }


  if(m_jump_state == 1)
    m_camera->move(glm::vec3(0,10,0)*delta );

  std::stringstream ss;
  ss << "   Energy: "<<m_energy ;
  m_lower_text->setText(ss.str());
}

void Player::refreshItemPosition()
{
  float world_time = Game::getGameTime();

  float seperation = 360.0f / AMOUNT_SPARKS;

  float cur_angle = 0;

  for(int x=0; x < AMOUNT_SPARKS; x++ )
  {
    if(m_available_sparks.size() > x)
    {

      glm::vec3 individual_offset = m_camera->getRight() *std::sin(world_time + glm::radians(cur_angle) ) *0.1f + m_camera->getDirection()* std::cos(world_time  + glm::radians(cur_angle))*0.1f; // glm::vec3( std::sin(world_time) *0.1f,  0, std::cos(world_time)*0.1f  );

      m_available_sparks[x]->setPosition( getPosition() +m_camera->getDirection()*0.5f +m_camera->getRight()*0.3f -m_camera->getUp() *0.2f + individual_offset   );

      cur_angle += seperation;
    }
  }

}

void Player::refreshSparks()
{
  //std::cout << "refresh" <<std::endl;
  Item * item  = m_inventory->getSelectedItem();
  if( item !=  nullptr)
  {
    float needed = item->getNeededEnergy();

    if(needed != -1.0f)
    {

      int amount = (int)m_energy / (int)needed;
      //std::cout <<"available "<<m_energy<<"/"<<needed<<" = "<< amount <<std::endl;


      if( amount < m_available_sparks.size() ) // less than the active ones needed
      {
        for(int x = 0; x< amount ; x++)
        {            
          m_available_sparks_animation[x] = 1;
          m_available_sparks[x]->setVisible();
        }
        for(int x = amount; x< m_available_sparks.size() ; x++)
        {
          m_available_sparks_animation[x] = 2;
        }
      }
      else                                 // moee than the active ones needed
      {
        for(int x=0; x < amount;x++)
        {
          if( x < m_available_sparks.size() )
          {
            m_available_sparks[x]->setVisible();     
            m_available_sparks_animation[x] = 1;
          }
          else
          {
            if( x < AMOUNT_SPARKS)
            {
              Object *  spark = Engine::getScene()->addObject("game/models/energy_remain.obj",getPosition() , false);
              Script * script = spark->addScript();
              RotationScriptElement * rse = script->addRotationScript();
              rse->setupAlways();
              rse->setup(glm::vec3(1000.0f,1000.0f,1000.0f));
              m_available_sparks.push_back(spark);
              m_available_sparks_animation[ x ] = 1;
            }
          }
        }
      }
    }


  }
}

Object * Player::getClosestSpark(glm::vec3 p)
{
  Object * ret = nullptr;
  float closest_dir = 100000;


  for(std::vector<Object *>::iterator it =m_available_sparks.begin(); it != m_available_sparks.end(); it++  )
  {
    if( (*it)->isVisible() )
    {
      float d = glm::distance( (*it)->getPosition(), p);
      if( d < closest_dir )
      {
        ret = (*it);
        closest_dir = d;
      }
    }
  }
  for(std::vector<Object *>::iterator it =m_available_sparks.begin(); it != m_available_sparks.end();   )
  {
    if( (*it) == ret )
      m_available_sparks.erase( it );
    else
      it++;
  }

  return ret;
}

void Player::cameraMovedCallback()
{
  glm::vec3 r(m_camera->getYaw(),-m_camera->getPitch()-90.0f,0);
  glm::vec3 d = m_camera->getDirection();
  //d.z = 0.0f;
  // d.x = 0.0f;


  Item * item = m_inventory->getSelectedItem();
  if(item != nullptr)
  {
    item->setPosition(m_camera->getPosition()+m_camera->getDirection()*0.5f +m_camera->getRight()*0.3f -m_camera->getUp() *0.2f);
    item->setRotation(r);
  }
  m_player_object->setPosition( m_camera->getPosition() );
  refreshItemPosition();
 // m_weapon->setRotation(r);
}

void Player::cameraRotatedCallback()
{
  glm::vec3 r(m_camera->getYaw(),-m_camera->getPitch()-90.0f,0);
  glm::vec3 d = m_camera->getDirection();

  Item * item = m_inventory->getSelectedItem();
  if(item != nullptr)
  {
    item->setPosition(m_camera->getPosition()+m_camera->getDirection()*0.5f +m_camera->getRight()*0.3f -m_camera->getUp() *0.2f);
    item->setRotation(r);
  }
  float range = 5.0f;
  Ray r2( Engine::getScene()->getCamera()->getPosition() , Engine::getScene()->getCamera()->getDirection(), 0.0f, range);
  float distance2 =0.0f;
  bool res = Engine::getPhysicSubsystem()->collisionRayScene(Engine::getScene(), &r2, &distance2);
  if( res && ( distance2 < 5.0f )  )
  {
    m_crosshair_active->setActive();
    m_crosshair_inactive->setInactive();
  }
  else
  {
    m_crosshair_inactive->setActive();
    m_crosshair_active->setInactive();
  }


  refreshItemPosition();
}

Object * Player::getWeapon()
{

  Item * item = m_inventory->getSelectedItem();
  if(item != nullptr)
  {
    if(item->isWeapon())
      return item->getObject();
  }
  return nullptr;
}

glm::vec3 Player::getPosition()
{
	return m_camera->getPosition();
}



void Player::checkAction(Environment * environment)
{
  if(m_inventory->getSelectedItem() != nullptr)
    m_inventory->getSelectedItem()->doAction(environment);
}
void Player::checkSecondaryAction(Environment * environment)
{
  if(m_inventory->getSelectedItem() != nullptr)
    m_inventory->getSelectedItem()->doSecondaryAction(environment);
}

void Player::drainEnergy(float val, bool deadly)
{
  m_energy-=val;
  if(deadly && (m_energy <0.0f) )
  {
    Game::end();
  }
  if( m_energy < 0.0f)
    m_energy = 0.0f;

  float scale = 1.0f/100.0f *   m_energy ;
  m_player_object->setScale(glm::vec3(scale,scale,scale));
  refreshSparks();
}

void Player::gainEngery(float val)
{
  SoundManager::addAndPlaySound("game/models/sounds/healspell1.wav");
  m_energy += val;
  
  float scale = 1.0f/100.0f *   m_energy ;
  m_player_object->setScale(glm::vec3(scale,scale,scale));
  refreshSparks();
}

float Player::getEnergy()
{
  return m_energy;
}

void Player::mouseScrollCallback(double x , double y)
{
  m_inventory->scroll(y);
}