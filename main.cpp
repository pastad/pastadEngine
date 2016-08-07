#include <stdio.h>
#include <iostream>
#include "Engine.h"

#include "Scene.h"
#include "Object.h"
#include "Terrain.h"
#include "Light.h"
#include "Wave.h"
#include "Water.h"
#include "IOSubsystem.h"

#include "Helper.h"

#include "GUI.h"
#include "Text.h"
#include "Image.h"
#include "Button.h"
#include <glm/glm.hpp>


#include "Game.h"
#include "GameMenu.h"

#include "LuaScript.h"

void test_callback(Button * b)
{
  std::cout << "woho callback"<<std::endl;
}


int main(void)
{

  Engine engine;



  bool launch_game = true;

  if(launch_game)
  {

    engine.initialize(1240, 720, PHYSIC_SUBSYSTEM | AUDIO_SUBSYSTEM, false, false);

    
    GameMenu * game_menu = new GameMenu();   
    Game * game = nullptr;
 
    game_menu->initialize();

    while(game_menu->isActive())
    {
      game_menu->update();
      engine.update();
      engine.render();
    }

    game_menu->unload();

    if( game_menu->shouldGameBeStarted())
    {
      Game * game = new Game();

      if(game->initialize())
      {

        while(engine.running())
        {        
          game->update(); 
          engine.update();            
          engine.render();
        }
      }

    }
    delete game_menu;
    

    engine.shutDown();

    if(game != nullptr)
      delete game;

  }
  else
  {
    engine.initialize(1240, 720, PHYSIC_SUBSYSTEM, true, false);
    Scene *  scene = new Scene();


   // LuaScript script;
   // script.loadFile("game/lua_scripts/test.lua");
   // script.executeMain();
   // script.callFunction("test");

    //Object * object5 = scene.addObject("models/sponza/sponza.obj"); 
    //Object * object6 = scene.addObject("models/table_medieval_trestle.obj"); 
     //object6->setPosition(glm::vec3(10,0,0));


    //Light * light3 = scene.addLight();
    //light3->setSpot(glm::vec3(0,5,0),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),
    //  1.0f,1.0f,0.09f,0.032f, 45.0f, glm::vec3(1,0,1) );


//    Light * light111 = scene.addLight();
//    light111->setPoint(glm::vec3(2,0,0),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),0.5f,0.1f,0.09f,0.032f,true);
//    Light * light112 = scene.addLight();
//    light112->setPoint(glm::vec3(2,4,0),glm::vec3(1,0,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),0.5f,0.1f,0.09f,0.032f,true);
   // Light * world_light = scene.addLight();
   // world_light->setDirectional(glm::vec3(0,-1,1), glm::vec3(1,0.95,0.9) ,glm::vec3(1,1,1), glm::vec3(1,1,1),0.3f);

    //scene->setSkybox("models/skybox1/sk1");


  /*  Object * fox = scene.addObject("game/models/Fox.dae", glm::vec3(4,0,7)); 
    fox->setRotation(glm::vec3(0.0f,90.0f,90.0f));
    fox->setScale(glm::vec3(0.5f,0.5f,0.5f));
    fox->applyPhysics();

    Object * bush = scene.addObjectInstanced("game/models/bush1.obj",glm::vec3(-4,0,5) ); 

    Object * bush2 = scene.addObjectInstanced("game/models/bush1.obj",glm::vec3(20,0,5) ); 

    Object * tree = scene.addObject("game/models/tree1.obj",glm::vec3(2,0,10) ); 
    Object * bridge = scene.addObject("game/models/bridge1.obj",glm::vec3(-2,0,15) ); 


    Object * ground = scene.addObject("models/ground.obj", glm::vec3(0,0,0)); 
    ground->setScale(glm::vec3(10.0f,10.f,10.0f));

    Object * gaz = scene.addObject("models/gaz.obj", glm::vec3(10,0,0)); 


    Object * temp_tree;

    for(int k=0; k< 20; k++)
    {
      for(int t=0; t< 20; t++)
      {
        temp_tree = scene.addObjectInstanced("game/models/tree1.obj",glm::vec3(t*4-10,0,k*4+10) ); 
      }      
    }*/
    Water * water_1  = scene->addWaterEffect(glm::vec3(0,1,0),6);
    Wave * wave_1 = water_1->addWave(glm::vec2(1,1),1.0f,0.001,0.1);
    Wave * wave_2 = water_1->addWave(glm::vec2(0,1),1.0f,0.001,0.1);


    //Object * test_roach = scene.addObject("models/rooster.dae", glm::vec3(0,1,7)); 
    //test_roach->setRotation(glm::vec3(0.0f,90.0f,90.0f));
   // test_roach->setScale(glm::vec3(0.1f,0.1f,0.1f));

    //Terrain * terrain =  scene.addTerrain();
    //terrain->generate();
    //scene.save("scene_save_test.xml");
    scene->load("island-scene.xml");
  //scene->load("scene2.xml");
   // scene->save("scene_save_test.xml");
    Helper::m_debug_float = 0.0f;

    engine.setScene(scene, false);
    //ground->setPriorityRender();  

    // run the main loop
    while(engine.running())
    {
      // do stuff here
      if(IOSubsystem::isKeyPressed('X') )
      {
        Helper::m_debug_float+=0.1f;
      }
      if(IOSubsystem::isKeyPressed('Z') )
      {
        Helper::m_debug_float-=0.1f;
        if(Helper::m_debug_float<0.0f)
          Helper::m_debug_float =0.0f;
      }

  
      if(IOSubsystem::isKeyPressedAndReleased('N') )
      {
        std::cout<< "N pressed:" <<std::endl;
        //if(fox->isVisible())
        ///  fox->setInvisible();
        //else
        //  fox->setVisible();
      }
      if(IOSubsystem::isKeyPressed('Q') )
      {
        Object * o = Engine::pickObjectAtCenter();
        Light * l = Engine::pickLightAt(glm::vec2(512,512) );
        if( o  != nullptr)
         std::cout << o->getIdentifier() <<std::endl;
        else
          std::cout << "no object picked" <<std::endl;
        if( l  != nullptr)
          std::cout << "light "<<l->getId()<<" picked" <<std::endl;
      }


      engine.update();
    	engine.render();
    }
    
    engine.shutDown();
    if(scene != nullptr)
      delete scene;
  }


  return 0;  
}
