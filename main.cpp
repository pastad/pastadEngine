#include <stdio.h>
#include <iostream>
#include "Engine.h"

#include "Scene.h"
#include "Object.h"
#include "Terrain.h"
#include "Light.h"
#include "IOSubsystem.h"

#include "Helper.h"

#include "GUI.h"
#include "Text.h"
#include "Image.h"
#include "Button.h"
#include <glm/glm.hpp>


#include "Game.h"


void test_callback(Button * b)
{
  std::cout << "woho callback"<<std::endl;
}


int main(void)
{

  Engine engine;

  engine.initialize(1240, 720, PHYSIC_SUBSYSTEM, true);

  bool launch_game = false;

  if(launch_game)
  {
    Game * game = new Game();

    if(game->initialize())
    {

      while(engine.running())
      {       
        engine.update();
        game->update();
        engine.render();
      }
    }

    delete game;
  }
  else
  {

    Scene scene;

    //Object * object5 = scene.addObject("models/sponza/sponza.obj"); 
    //Object * object6 = scene.addObject("models/table_medieval_trestle.obj"); 
     //object6->setPosition(glm::vec3(10,0,0));


    //Light * light3 = scene.addLight();
    //light3->setSpot(glm::vec3(0,5,0),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),
    //  1.0f,1.0f,0.09f,0.032f, 45.0f, glm::vec3(1,0,1) );


    Light * light111 = scene.addLight();
    light111->setPoint(glm::vec3(2,0,0),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),0.5f,0.1f,0.09f,0.032f);
    Light * world_light = scene.addLight();
    world_light->setDirectional(glm::vec3(0,-1,1), glm::vec3(1,0.95,0.9) ,glm::vec3(1,1,1), glm::vec3(1,1,1),0.3f);

    scene.setSkybox("models/skybox1/sk1");


    Object * fox = scene.addObject("game/models/Fox.dae", glm::vec3(4,0,7)); 
    fox->setRotation(glm::vec3(0.0f,90.0f,90.0f));
    fox->setScale(glm::vec3(0.5f,0.5f,0.5f));
    fox->applyPhysics();

    Object * bush = scene.addObjectInstanced("game/models/bush1.obj",glm::vec3(-4,0,5) ); 

    Object * bush2 = scene.addObjectInstanced("game/models/bush1.obj",glm::vec3(20,0,5) ); 

    Object * tree = scene.addObject("game/models/tree1.obj",glm::vec3(2,0,10) ); 
    Object * bridge = scene.addObject("game/models/bridge1.obj",glm::vec3(-2,0,15) ); 


    Object * ground = scene.addObject("game/models/test_ground.obj", glm::vec3(0,0,0)); 

    Object * gaz = scene.addObject("models/gaz.obj", glm::vec3(10,0,0)); 

    //Object * test_roach = scene.addObject("models/rooster.dae", glm::vec3(0,1,7)); 
    //test_roach->setRotation(glm::vec3(0.0f,90.0f,90.0f));
   // test_roach->setScale(glm::vec3(0.1f,0.1f,0.1f));

    Terrain * terrain =  scene.addTerrain();
   
    Helper::m_debug_float = 0.0f;

    engine.setScene(&scene);

    ground->setPriorityRender();  

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

      if(IOSubsystem::isKeyPressed('F') )
      {
        light111->setPosition( light111->getPosition() + glm::vec3(0.05f,0.0f,0.0f)  );
      }
      if(IOSubsystem::isKeyPressed('H') )
      {
        light111->setPosition( light111->getPosition() - glm::vec3(0.05f,0.0f,0.0f)  );
      }
      if(IOSubsystem::isKeyPressed('T') )
      {
        light111->setPosition( light111->getPosition() + glm::vec3(0.0f,0.0f,0.05f)  );
      }
      if(IOSubsystem::isKeyPressed('B') )
      {
        light111->setPosition( light111->getPosition() - glm::vec3(0.0f,0.0f,0.05f)  );
      }
      if(IOSubsystem::isKeyPressed('N') )
      {
        if(fox->isVisible())
          fox->setInvisible();
        else
          fox->setVisible();
      }
      if(IOSubsystem::isKeyPressed('Q') )
      {
        Object * o = Engine::pickObjectAtCenter();
        if( o  != nullptr)
         std::cout << o->getIdentifier() <<std::endl;
        else
          std::cout << "no object picked" <<std::endl;
      }
      engine.update();
    	engine.render();
    }
  }

  engine.shutDown();

  return 0;  
}
