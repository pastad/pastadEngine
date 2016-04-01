#include <stdio.h>
#include <iostream>
#include "Engine.h"

int main(void)
{
  Engine engine;

  engine.initialize(800, 600);
  
  while(engine.running())
  {
  	engine.update();
  	engine.beginRender();

  	engine.endRender();
  }

  engine.shutDown();

  return 0;  
}
