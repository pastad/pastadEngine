#include "GameMenu.h"

#include "Game.h"

#include "Engine.h"
#include "GUI.h"
#include "Button.h"
#include "Image.h"
#include "Text.h"


#define SLIDE_SIDE_OFFSET 1000.0f
#define SLIDE_TOP_OFFSET 400

#include <iostream>
#include <sstream>

GameMenu::GameMenu():m_launch_game(true),m_selected(0)
{
}
GameMenu::~GameMenu()
{  
}

bool GameMenu::initialize(float width, float height)
{

  m_spec_size = SPEC_SIZE_1920_1080 ;
  m_spec_fullscreen =  true;
  m_spec_shadow = ST_STANDARD_PCF;
  m_spec_sound_effect_volume = 50;
  m_spec_sound_background_volume = 50;

  Game::readConfig("game/game_config.xml", &m_spec_sound_effect_volume, & m_spec_sound_background_volume);


  unsigned int wwidth;
  unsigned int wheight;
  bool edit;
  unsigned int flags;
  ShadowTechniqueType shadow ;

  Engine::readConfig( "game/engine_config.xml", &wwidth,  &wheight, &m_spec_fullscreen, &edit, &flags , &shadow);
  m_spec_shadow  =  (unsigned int) shadow;

  if(wwidth == 1680)
    m_spec_size =SPEC_SIZE_1680_1050;
  if(wwidth == 1270)
    m_spec_size =SPEC_SIZE_1270_720;

  m_gui = Engine::addGUI();

  m_menu_background = m_gui->addImage();
  m_menu_background->load("game/graphics/menu_background.png");

  float back_scale_x = m_menu_background->getSize().x;
  float back_scale_y = m_menu_background->getSize().y;

  if(m_spec_size == SPEC_SIZE_1920_1080)
  {
    back_scale_x = 1920.0f / back_scale_x;
    back_scale_y = 1080.0f / back_scale_y;
  }
  if(m_spec_size == SPEC_SIZE_1680_1050)
  {
    back_scale_x = 1680.0f / back_scale_x;
    back_scale_y = 1050.0f / back_scale_y;
  }
  if(m_spec_size == SPEC_SIZE_1270_720)
  {
    back_scale_x = 1270.0f / back_scale_x;
    back_scale_y= 720.0f / back_scale_y;
  }
  //std::cout << back_scale_x<< "," <<back_scale_y<<std::endl;
  m_menu_background->setScale(glm::vec2(back_scale_x,back_scale_y));

  m_img_instructions = m_gui->addImage();
  m_img_instructions->load("game/graphics/Instructions.png");
  m_img_instructions->setInactive();

  m_img_credits = m_gui->addImage();
  m_img_credits->load("game/graphics/Credits.png");


  float ssize1 = width - (SLIDE_SIDE_OFFSET /2.0f);
  float ssize2 = height - SLIDE_TOP_OFFSET;

  float s1  = ( ssize1 / m_img_credits->getSize().x) *back_scale_x;
  float s2  = ( ssize2 / m_img_credits->getSize().y) *back_scale_y;

  m_img_credits->setInactive();
  m_img_credits->setScale(glm::vec2(  0.75f * back_scale_x , 0.75f *back_scale_y));
  m_img_credits->setPosition(glm::vec2( (SLIDE_SIDE_OFFSET/2.0f - 100.0f) *back_scale_x , Engine::getWindowHeight() - m_img_credits->getSize().y * back_scale_y ));

  m_img_instructions->setScale(glm::vec2(0.6f *back_scale_x ,0.6f *back_scale_y));
  m_img_instructions->setPosition(glm::vec2( (SLIDE_SIDE_OFFSET/2.0f - 100.0f ) *back_scale_x, Engine::getWindowHeight() - m_img_instructions->getSize().y * back_scale_y ));


  m_btn_play =  m_gui->addButton();
  m_btn_play->intitializeWithToggle("game/graphics/menu_play_toggled.png","game/graphics/menu_play.png",
  glm::vec2(Engine::getWindowWidth()/2.0f-100.0f*back_scale_x,Engine::getWindowHeight()/2.0f),  glm::vec2(0.3f,0.3f), "Play", true);
  m_btn_play->setScale(glm::vec2(back_scale_x *0.3f,back_scale_y*0.3f));

  m_btn_instructions =  m_gui->addButton();
  m_btn_instructions->intitializeWithToggle("game/graphics/menu_instructions_toggled.png","game/graphics/menu_instructions.png",
  glm::vec2(Engine::getWindowWidth()/2.0f-100.0f*back_scale_x,Engine::getWindowHeight()/2.0f-50.0f*back_scale_y),  glm::vec2(0.3f,0.3f), "Instructions", false);
  m_btn_instructions->setScale(glm::vec2(back_scale_x *0.3f,back_scale_y*0.3f));

  m_btn_options =  m_gui->addButton();
  m_btn_options->intitializeWithToggle("game/graphics/menu_options_toggled.png","game/graphics/menu_options.png",
  glm::vec2(Engine::getWindowWidth()/2.0f-100.0f*back_scale_x,Engine::getWindowHeight()/2.0f-100.0f*back_scale_y),  glm::vec2(0.3f,0.3f), "Options", false);
  m_btn_options->setScale(glm::vec2(back_scale_x *0.3f,back_scale_y*0.3f));

  m_btn_credits =  m_gui->addButton();
  m_btn_credits->intitializeWithToggle("game/graphics/menu_credits_toggled.png","game/graphics/menu_credits.png",
  glm::vec2(Engine::getWindowWidth()/2.0f-100.0f*back_scale_x,Engine::getWindowHeight()/2.0f-150.0f*back_scale_y),  glm::vec2(0.3f,0.3f), "Credits", false);
  m_btn_credits->setScale(glm::vec2(back_scale_x *0.3f,back_scale_y*0.3f));

  m_btn_exit =  m_gui->addButton();
  m_btn_exit->intitializeWithToggle("game/graphics/menu_exit_toggled.png","game/graphics/menu_exit.png",
  glm::vec2(Engine::getWindowWidth()/2.0f-100.0f*back_scale_x,Engine::getWindowHeight()/2.0f-200.0f*back_scale_y),  glm::vec2(0.3f,0.3f), "Exit", false);
  m_btn_exit->setScale(glm::vec2(back_scale_x *0.3f,back_scale_y*0.3f));

  m_txt_options_size = m_gui->addText();
  m_txt_options_size->set( "Window size", glm::vec2(Engine::getWindowWidth()/2.0f-250.0f*back_scale_x,Engine::getWindowHeight()/2.0f), 0.5f*back_scale_x, glm::vec3(0,0,0) );
  m_txt_options_size->setInactive();

  m_txt_options_fullscreen = m_gui->addText();
  m_txt_options_fullscreen->set( "Fullscreen", glm::vec2(Engine::getWindowWidth()/2.0f-250.0f*back_scale_x,Engine::getWindowHeight()/2.0f - 25.0f*back_scale_y), 0.5f*back_scale_x, glm::vec3(0,0,0) );
  m_txt_options_fullscreen->setInactive();

  m_txt_options_shadows = m_gui->addText();
  m_txt_options_shadows->set( "Shadows", glm::vec2(Engine::getWindowWidth()/2.0f-250.0f*back_scale_x,Engine::getWindowHeight()/2.0f - 50.0f*back_scale_y), 0.5f*back_scale_x, glm::vec3(0,0,0));
  m_txt_options_shadows->setInactive();

  m_txt_options_sound_effect = m_gui->addText();
  m_txt_options_sound_effect->set( "Sound effect volume", glm::vec2(Engine::getWindowWidth()/2.0f-250.0f*back_scale_x,Engine::getWindowHeight()/2.0f - 75.0f*back_scale_y), 0.5f*back_scale_x, glm::vec3(0,0,0) );
  m_txt_options_sound_effect->setInactive();

  m_txt_options_sound_background = m_gui->addText();
  m_txt_options_sound_background->set( "Background sound volume", glm::vec2(Engine::getWindowWidth()/2.0f-250.0f*back_scale_x,Engine::getWindowHeight()/2.0f - 100.0f*back_scale_y), 0.5f*back_scale_x, glm::vec3(0,0,0));
  m_txt_options_sound_background->setInactive();

  m_txt_options_notice = m_gui->addText();
  m_txt_options_notice->set( "Changes will be applied after restart", glm::vec2(Engine::getWindowWidth()/2.0f-250.0f*back_scale_x, 100.0f*back_scale_y), 0.3f*back_scale_x, glm::vec3(0,0,0));
  m_txt_options_notice->setInactive();

  m_txt_highscore_last = m_gui->addText();
  m_txt_highscore_last->set( "Last game", glm::vec2(Engine::getWindowWidth()/2.0f , 50*back_scale_y), 0.4f*back_scale_x, glm::vec3(0.67,0.67,0.67));
  m_txt_highscore_last->setInactive();
  m_txt_highscore_best = m_gui->addText();
  m_txt_highscore_best->set( "Best game", glm::vec2(Engine::getWindowWidth()/2.0f , 25.0f*back_scale_y), 0.4f*back_scale_x, glm::vec3(0.67,0.67,0.67));
  m_txt_highscore_best->setInactive();


  HighScore last;
  HighScore best;

  Game::readHighsore( "game/game_highscore.xml", &last, &best);

  if( last.time != 0.0f )
  {
   // m_txt_highscore_last->setActive();
   // m_txt_highscore_best->setActive();
    m_txt_highscore_last->setCentered();
    m_txt_highscore_best->setCentered();

    std::stringstream ss;
    ss << "Last game: "<<last.days<<"days, "<<last.time << " seconds, " << last.energy_left<< " remaining energy";

    m_txt_highscore_last->setText(ss.str());


    ss.clear(); ss.str("");
    ss << "Best game: "<<best.days<<"days, "<<best.time << " seconds, " << best.energy_left<< " remaining energy";
    m_txt_highscore_best->setText(ss.str());
  }

  m_active = true;

  m_gui->registerButtonPressedCallback(&mouseButtonCallback);

  m_background_sound_buffer = new sf::SoundBuffer();
  m_background_sound_sound = new sf::Sound();

  if (!m_background_sound_buffer->loadFromFile("game/sounds/litm.flac"))
  {
    delete m_background_sound_sound;
    delete m_background_sound_buffer;
    return false;
  }

  m_background_sound_sound->setBuffer(*m_background_sound_buffer);
  m_background_sound_sound->play();
  m_background_sound_sound->setLoop(true);
  m_background_sound_sound->setVolume(m_spec_sound_background_volume);

  m_selected =0;
  m_selected_slide = GM_SLIDE_MAIN;
  m_main_state = GM_STATE_FADE_IN;
  m_main_state_timer = 0.0f;



  Game::readConfig("game/game_config.xml", &m_spec_sound_effect_volume, &m_spec_sound_background_volume );
  m_background_sound_sound->setVolume(m_spec_sound_background_volume);

  std::cout << "menu loaded" <<std::endl;

  return true;
}

bool GameMenu::unload()
{
  Engine::removeGUI(m_gui);

  m_background_sound_sound->stop();

  delete m_background_sound_sound;
  delete m_background_sound_buffer;
}

void GameMenu::update()
{
  if(m_active)
  {
    if( m_main_state == GM_STATE_FADE_IN)
    {
      float delta = Engine::getTimeDelta();
      m_main_state_timer += delta;

      if( m_main_state_timer > FADE_IN_TIME )
      {
        m_main_state = GM_STATE_ACTIVE;
        m_menu_background->mixWithColor(glm::vec3(1,1,1), 0.0f);
        m_main_state_timer = 0.0f;
        m_txt_highscore_last->setActive();
        m_txt_highscore_best->setActive();
      }
      else
      {
        float sc =  1.0f - ( ( 1.0f / FADE_IN_TIME ) *m_main_state_timer );

        m_menu_background->mixWithColor(glm::vec3(1,1,1), sc);
        m_btn_instructions->mixWithColor(glm::vec3(1,1,1), sc);        
        m_btn_options->mixWithColor(glm::vec3(1,1,1), sc);
        m_btn_play->mixWithColor(glm::vec3(1,1,1), sc);
        m_btn_credits->mixWithColor(glm::vec3(1,1,1), sc);
        m_btn_exit->mixWithColor(glm::vec3(1,1,1), sc);
      }


    }
    if( m_main_state == GM_STATE_ACTIVE)
    {
      if(m_selected_slide == GM_SLIDE_MAIN)
      {
        bool select_done = false;

        if( Engine::isKeyReleasedAndPressed(265))
        {
          select_done = true;  
          if(m_selected > 0)  
            m_selected--;
        }

        if( Engine::isKeyReleasedAndPressed(264))    
        {
          select_done = true;
          m_selected++;

          if(m_selected > 4)
            m_selected =  4;

        }        

        if(select_done)
          refreshSelection();
      }
      if(m_selected_slide == GM_SLIDE_OPTIONS)
      {
        bool select_done = false;

        if( Engine::isKeyReleasedAndPressed(265))
        {
          select_done = true;    
          if(m_selected > 0)  
            m_selected--;
        }

        if( Engine::isKeyReleasedAndPressed(264))    
        {
          select_done = true;
          m_selected++;

          if(m_selected > 4)
            m_selected =  4;

        }
      
        if(select_done)
          refreshSelection();

        if( Engine::isKeyReleasedAndPressed(263)) //left
        {
          if(m_selected == 0)
          {
            if(m_spec_size >0)
             m_spec_size--;
          }
          if(m_selected == 1)
          {
            m_spec_fullscreen = true;
          }
          if(m_selected == 2)
          {
            m_spec_shadow--;
            if(m_spec_shadow < 1 )
              m_spec_shadow =1;
          }
          if(m_selected == 3)
          {
            if( m_spec_sound_effect_volume > 0)
              m_spec_sound_effect_volume--;

          }
          if(m_selected == 4)
          {
            if(m_spec_sound_background_volume > 0)
             m_spec_sound_background_volume--;
            m_background_sound_sound->setVolume(m_spec_sound_background_volume);
          }
          refreshSelection();
        }
        if( Engine::isKeyReleasedAndPressed(262)) //right
        {
          if(m_selected == 0)
          {
            m_spec_size++;
            if(m_spec_size > SPEC_SIZE_1270_720 )
              m_spec_size =SPEC_SIZE_1270_720;
          }
          if(m_selected == 1)
          {
            m_spec_fullscreen = false;
          }
          if(m_selected == 2)
          {
            m_spec_shadow++;
            if(m_spec_shadow > ST_STANDARD_RS)
              m_spec_shadow = ST_STANDARD_RS;
          }
          if(m_selected == 3)
          {
            m_spec_sound_effect_volume++;
            if( m_spec_sound_effect_volume >100 ) 
              m_spec_sound_effect_volume = 100;
          }
          if(m_selected == 4)
          {
            m_spec_sound_background_volume++;
            if( m_spec_sound_background_volume >100 ) 
              m_spec_sound_background_volume = 100;
            m_background_sound_sound->setVolume(m_spec_sound_background_volume);
          }
          refreshSelection();
        }
      }


      if( Engine::isKeyReleasedAndPressed(257) || Engine::isKeyReleasedAndPressed(335) ) //335
      {
        if(m_selected_slide ==   GM_SLIDE_MAIN)
        {
          if(m_btn_play->isToggled())
          {
            m_launch_game = true;
            m_main_state = GM_STATE_FADE_OUT;            
            m_txt_highscore_last->setInactive();
            m_txt_highscore_best->setInactive();
          }
          if(m_btn_instructions->isToggled())
          {
            changeSlide(GM_SLIDE_INSTRUCTIONS);
          }
          if(m_btn_options->isToggled())
          {
            m_selected =0;       
            changeSlide(GM_SLIDE_OPTIONS);                 
            refreshSelection();
          }
          if(m_btn_credits->isToggled())
          {
            changeSlide(GM_SLIDE_CREDITS);
          }
          if(m_btn_exit->isToggled())
          {
            m_launch_game = false;
            m_main_state = GM_STATE_FADE_OUT;
            m_txt_highscore_last->setInactive();
            m_txt_highscore_best->setInactive();
          }
        }
        else
        {
          if(m_selected_slide ==  GM_SLIDE_INSTRUCTIONS)          
            changeSlide(GM_SLIDE_MAIN);          
          else
          {
            if(m_selected_slide ==  GM_SLIDE_CREDITS)            
              changeSlide(GM_SLIDE_MAIN);            
            else
            {
              if(m_selected_slide == GM_SLIDE_OPTIONS)
              {
                m_selected =0;

                // options left -> save them
                if (SPEC_SIZE_1920_1080 == m_spec_size) 
                  Engine::saveConfig("game/engine_config.xml", 1920,1080,m_spec_fullscreen, 0, RENDER_SUBSYSTEM | IO_SUBSYSTEM | PHYSIC_SUBSYSTEM, (ShadowTechniqueType) m_spec_shadow  );
                if (SPEC_SIZE_1680_1050 == m_spec_size) 
                  Engine::saveConfig("game/engine_config.xml", 1680,1050,m_spec_fullscreen, 0, RENDER_SUBSYSTEM | IO_SUBSYSTEM | PHYSIC_SUBSYSTEM, (ShadowTechniqueType) m_spec_shadow  );
                if (SPEC_SIZE_1270_720 == m_spec_size) 
                  Engine::saveConfig("game/engine_config.xml", 1270,720,m_spec_fullscreen, 0, RENDER_SUBSYSTEM | IO_SUBSYSTEM | PHYSIC_SUBSYSTEM, (ShadowTechniqueType) m_spec_shadow  );
                changeSlide(GM_SLIDE_MAIN);
              }
            }
          }
        }
      }
    }
    if( m_main_state == GM_STATE_FADE_OUT)
    {
      float delta = Engine::getTimeDelta();
      m_main_state_timer += delta;

      if( m_main_state_timer > FADE_IN_TIME )
      {
        m_menu_background->mixWithColor(glm::vec3(1,1,1), 1.0f);
        m_main_state_timer = 0.0f;
        m_active = false;
      }
      else
      {
        float sc =   ( ( 1.0f / FADE_IN_TIME ) *m_main_state_timer );
        m_menu_background->mixWithColor(glm::vec3(1,1,1), sc);
        m_btn_instructions->mixWithColor(glm::vec3(1,1,1), sc);
        m_btn_play->mixWithColor(glm::vec3(1,1,1), sc);
        m_btn_options->mixWithColor(glm::vec3(1,1,1), sc);
        m_btn_credits->mixWithColor(glm::vec3(1,1,1), sc);
        m_btn_exit->mixWithColor(glm::vec3(1,1,1), sc);
      }
    }
  }
}



void GameMenu::refreshSelection()
{
  if(m_selected_slide == GM_SLIDE_MAIN)
  {
    if(m_selected == 0)
    {
      if(!m_btn_play->isToggled())
      {
        m_btn_play->togglOn();
        m_btn_instructions->togglOff();
        m_btn_options->togglOff();
        m_btn_credits->togglOff();
        m_btn_exit->togglOff();
      }
    }
    if(m_selected == 1)
    {
      if(!m_btn_instructions->isToggled())
      {
        m_btn_play->togglOff();
        m_btn_instructions->togglOn();
        m_btn_options->togglOff();
        m_btn_credits->togglOff();
        m_btn_exit->togglOff();
      }
    }
    if(m_selected == 2)
    {
      if(!m_btn_options->isToggled())
      {
        m_btn_play->togglOff();
        m_btn_instructions->togglOff();
        m_btn_options->togglOn();
        m_btn_credits->togglOff();
        m_btn_exit->togglOff();
      }
    }
    if(m_selected == 3)
    {
      if(!m_btn_credits->isToggled())
      {
        m_btn_play->togglOff();        
        m_btn_instructions->togglOff();
        m_btn_options->togglOff();
        m_btn_credits->togglOn();
        m_btn_exit->togglOff();
      }
    }
     if(m_selected == 4)
    {
      if(!m_btn_exit->isToggled())
      {
        m_btn_play->togglOff();        
        m_btn_instructions->togglOff();
        m_btn_options->togglOff();
        m_btn_credits->togglOff();
        m_btn_exit->togglOn();
      }
    }
  }
  if(m_selected_slide == GM_SLIDE_OPTIONS)
  { 
    std::stringstream ss;

    if( m_selected == 0 )
      ss << "<>Window Size ";
    else
      ss << " Window Size ";

    if(m_spec_size == SPEC_SIZE_1270_720)
      ss << "1270 x 720";
    if(m_spec_size == SPEC_SIZE_1680_1050)
      ss << "1680 × 1050";
    if(m_spec_size == SPEC_SIZE_1920_1080)
      ss << "1920 × 1200";

    m_txt_options_size->setText(ss.str());

    ss.clear();
    ss.str("");

    if( m_selected == 1 )
      ss << "<>Fullscreen ";
    else
      ss << " Fullscreen ";

    if(m_spec_fullscreen)
      ss << "on";
    else
      ss << "off";

     m_txt_options_fullscreen->setText(ss.str());
    ss.clear();
    ss.str("");   

    if( m_selected == 2 )
      ss << "<>Sound effect ";
    else
      ss << " Sound effect ";
  
    if(m_spec_shadow == ST_STANDARD)
      ss << "Standard";
    if(m_spec_shadow == ST_STANDARD_PCF)
      ss << "Percentage-Close filtering";
    if(m_spec_shadow == ST_STANDARD_RS)
      ss << "Random Sampling";

    m_txt_options_shadows->setText(ss.str());
    ss.clear();
    ss.str("");   

    if( m_selected == 3 )
      ss << "<>Sound effect ";
    else
      ss << " Sound effect ";
    ss << m_spec_sound_effect_volume;

    m_txt_options_sound_effect->setText(ss.str());
    ss.clear();
    ss.str("");   

    if( m_selected == 4 )
      ss << "<>Background volume ";
    else
      ss << " Background volume ";
    ss << m_spec_sound_background_volume;

    m_txt_options_sound_background->setText(ss.str());
    ss.clear();
    ss.str(""); 

 /*   if(m_selected == 0)
    {
      m_txt_options_shadows->setText("  Shadows");
      m_txt_options_sound_effect->setText("  Sound effect volume");
      m_txt_options_sound_background->setText("  Background volume");
    }
    if(m_selected == 1)
    {
      m_txt_options_shadows->setText("  Shadows");
      m_txt_options_sound_effect->setText("  Sound effect volume");
      m_txt_options_sound_background->setText("  Background volume");
    }
    if(m_selected == 2)
    {
      m_txt_options_shadows->setText("<>Shadows");
      m_txt_options_sound_effect->setText("  Sound effect volume");
      m_txt_options_sound_background->setText("  Background volume");
    }
    if(m_selected == 3)
    {
      m_txt_options_shadows->setText("  Shadows");
      m_txt_options_sound_effect->setText("<>Sound effect volume");
      m_txt_options_sound_background->setText("  Background volume");
    }
    if(m_selected == 4)
    {
      m_txt_options_shadows->setText("  Shadows");
      m_txt_options_sound_effect->setText("  Sound effect volume");
      m_txt_options_sound_background->setText("<>Background volume");
    }*/
    
  }
}


void GameMenu::changeSlide(unsigned int slide)
{
  m_selected_slide = slide;

  if(slide == GM_SLIDE_MAIN)
  {
    m_btn_play->setActive();
    m_btn_credits->setActive();
    m_btn_instructions->setActive();
    m_btn_exit->setActive();
    m_btn_options->setActive();

    m_img_instructions->setInactive();

    m_img_credits->setInactive();

    deactivateOptions();

    refreshSelection();
  }
  if(slide == GM_SLIDE_INSTRUCTIONS)
  {
    // std::cout << "Instructions" <<std::endl;
    m_btn_play->setInactive();
    m_btn_credits->setInactive();
    m_btn_instructions->setInactive();
    m_btn_exit->setInactive();
    m_btn_options->setInactive();

    m_img_instructions->setActive();

    deactivateOptions();

    m_img_credits->setInactive();
  }
  if(slide == GM_SLIDE_CREDITS)
  {
    m_btn_play->setInactive();
    m_btn_credits->setInactive();
    m_btn_instructions->setInactive();
    m_btn_exit->setInactive();
    m_btn_options->setInactive();

    m_img_instructions->setInactive();

    deactivateOptions();

    m_img_credits->setActive();
  }
  if(slide == GM_SLIDE_OPTIONS)
  {
    m_btn_play->setInactive();
    m_btn_credits->setInactive();
    m_btn_instructions->setInactive();
    m_btn_exit->setInactive();
    m_btn_options->setInactive();

    m_img_instructions->setInactive();

    activateOptions();

    m_img_credits->setInactive();
  }
}

void GameMenu::activateOptions()
{
  m_txt_options_size->setActive();
  m_txt_options_fullscreen->setActive();
  m_txt_options_shadows->setActive();
  m_txt_options_sound_effect->setActive();
  m_txt_options_sound_background->setActive();
  m_txt_options_notice->setActive();

}
void GameMenu::deactivateOptions()
{  
  m_txt_options_size->setInactive();
  m_txt_options_fullscreen->setInactive();
  m_txt_options_shadows->setInactive();
  m_txt_options_sound_effect->setInactive();
  m_txt_options_sound_background->setInactive();
  m_txt_options_notice->setInactive();

}

bool GameMenu::isActive()
{
  return m_active;
}

bool GameMenu::shouldGameBeStarted()
{
  return m_launch_game;
}

void GameMenu::mouseButtonCallback(Button * btn)
{
  if(btn->getDescriptor() == "Play")
    std::cout <<"Play pressed"<<std::endl;
}
