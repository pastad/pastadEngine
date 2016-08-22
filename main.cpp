#define NDEBUG

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


int main(void)
{
  Engine engine;

  // intit the engine
	engine.initialize(1240, 720, RENDER_SUBSYSTEM , true, false);

  // load and set the start scene
	Scene *  scene = new Scene();
	scene->load("island-scene.xml");
	engine.setScene(scene, false);

  // run the main loop
  while(engine.running())
  {
    engine.update();
    engine.render();    
  }
   
   // shut everything down
  engine.shutDown();

  if(scene != nullptr)
    delete scene;  


  return 0;  
}
