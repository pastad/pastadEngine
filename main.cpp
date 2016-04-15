#include <stdio.h>
#include <iostream>
#include "Engine.h"

#include "Scene.h"
#include "Object.h"

int main(void)
{
  Engine engine;

  engine.initialize(800, 600, 0);

  Scene scene;
  Object * object = scene.addObject("models/cube2.obj"); 

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
