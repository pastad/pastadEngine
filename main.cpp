#include <stdio.h>
#include <iostream>
#include "Engine.h"

#include "Scene.h"
#include "Object.h"
#include "Light.h"

int main(void)
{
  Engine engine;

  engine.initialize(800, 600, 0);

  Scene scene;
  Object * object = scene.addObject("models/cube2.obj"); 
  Object * object2 = scene.addObject("models/bush.obj");
  Object * object3 = scene.addObject("models/base.obj"); 
  Light * light = scene.addLight();
  light->setDirectional(glm::vec3(0,0,1), glm::vec3(1,1,1),glm::vec3(1,1,1), glm::vec3(1,1,1),1.0f);
  //light->setPoint(glm::vec3(2,0,0),glm::vec3(1,1,1),glm::vec3(1.0,1.0,1.0),glm::vec3(1,1,1),1.0f,1.0f,0.09f,0.032f);
  //light->setSpot(glm::vec3(2,0,0),glm::vec3(1,1,1),glm::vec3(1,1,1),glm::vec3(1,1,1),1.0f,1.0f,0.09f,0.032f,45.0f,glm::vec3(-1,0,0));
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
