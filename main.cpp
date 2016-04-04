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
    // do stuff here

  	engine.update();
  	engine.render();
  }

  engine.shutDown();

  return 0;  
}
