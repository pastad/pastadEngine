#include <stdio.h>
#include <iostream>
#include "Engine.h"

int main(void)
{
  Engine engine;

  engine.initialize(800, 600, 0);
  
  // run the main loop
  while(engine.running())
  {
  	engine.update();
  	engine.beginRender();

  	// render stuff here
  	// should be later done by engine through a scene


  	engine.endRender();
  }

  engine.shutDown();

  return 0;  
}
