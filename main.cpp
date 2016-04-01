#include <stdio.h>
#include <iostream>
#include "Engine.h"

int main(void)
{
  Engine engine;

  engine.initialize(800, 600);
  
  engine.shutDown();

  return 0;  
}
