#include <stdio.h>
#include <iostream>
#include "Engine.h"

#include "Scene.h"
#include "Object.h"
#include "Light.h"
#include "IOSubsystem.h"


#include "GUI.h"
#include "Text.h"
#include "Image.h"
#include "Button.h"

void test_callback(Button * b)
{
  std::cout << "woho callback"<<std::endl;
}

int main(void)
{
  Engine engine;

  engine.initialize(1240, 720, 0);

  Scene scene;

  //Object * object5 = scene.addObject("models/sponza/sponza.obj"); 
  //Object * object6 = scene.addObject("models/table_medieval_trestle.obj"); 
   //object6->setPosition(glm::vec3(10,0,0));


//  Light * light3 = scene.addLight();
//  light3->setSpot(glm::vec3(0,5,0),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),
//    1.0f,1.0f,0.09f,0.032f, 45.0f, glm::vec3(0,0,1) );


  Light * light111 = scene.addLight();
  light111->setPoint(glm::vec3(2,2,0),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),1.0f,1.0f,0.09f,0.032f);


  
  scene.setSkybox("models/skybox1/sk1");


  Object * ground = scene.addObject("game/models/test_ground.obj"); 

  Object * fox = scene.addObject("game/models/Fox.dae"); 
  fox->setPosition(glm::vec3(4,0,7));
  fox->setRotation(glm::vec3(0.0f,90.0f,90.0f));
  fox->setScale(glm::vec3(0.5f,0.5f,0.5f));

  Object * bush = scene.addObject("game/models/bush1.obj"); 
  bush->setPosition(glm::vec3(-4,0,5));

  engine.setScene(&scene);

  // run the main loop
  while(engine.running())
  {
    // do stuff here
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
    engine.update();
  	engine.render();
  }

  engine.shutDown();

  return 0;  
}
