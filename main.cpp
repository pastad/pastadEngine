#include <stdio.h>
#include <iostream>
#include "Engine.h"

#include "Scene.h"
#include "Object.h"
#include "Light.h"

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
  Object * object = scene.addObject("models/cube2.obj"); 
  Object * object2 = scene.addObject("models/bush.obj");
  Object * object3 = scene.addObject("models/base.obj"); 

  object->setPosition(glm::vec3(-2,0,6));
  object2->setPosition(glm::vec3(0,0,6));
  object3->setPosition(glm::vec3(0,-1,6));

  //Light * light = scene.addLight();
  Light * light2 = scene.addLight();
  //light->setDirectional(glm::vec3(0,0,1), glm::vec3(1,1,1),glm::vec3(1,1,1), glm::vec3(1,1,1),1.0f);
  light2->setPoint(glm::vec3(2,0,0),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),1.0f,1.0f,0.09f,0.032f);
  Light * light3 = scene.addLight();
  light3->setSpot(glm::vec3(0,0,0),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),
      1.0f,1.0f,0.09f,0.032f, 45.0f, glm::vec3(0,0,1) );
 // Light * light4 = scene.addLight();
 //  light4->setSpot(glm::vec3(5,0,0),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),
   //   1.0f,1.0f,0.09f,0.032f, 45.0f, glm::vec3(-1,0,1) );
 // Light * light5 = scene.addLight();
 // light5->setSpot(glm::vec3(0,2,0),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),
  //    1.0f,1.0f,0.09f,0.032f, 45.0f, glm::vec3(0,0,1) );

//Light * light6 = scene.addLight();
 // light6->setSpot(glm::vec3(-5,0,0),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),
  //    1.0f,1.0f,0.09f,0.032f, 45.0f, glm::vec3(1,0,1) );



  engine.setScene(&scene);

  // run the main loop
  while(engine.running())
  {
    // do stuff here

  	engine.update();
  	engine.render();
  }

  engine.shutDown();

  return 0;  
}
