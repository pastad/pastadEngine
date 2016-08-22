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


//#include "LuaScript.h"

#include <thread>
//#include <future>



void test_callback(Button * b)
{
  std::cout << "woho callback"<<std::endl;
}

void update(float d)
{

}

int main(void)
{

  Engine engine;

  unsigned int w = 1920;
  unsigned int h = 1080; 

  
	engine.initialize(1240, 720, RENDER_SUBSYSTEM , true, false);
	Scene *  scene = new Scene();

	scene->load("island-scene.xml");
	Helper::m_debug_float = 0.0f;

	engine.setScene(scene, false);

  // run the main loop
  while(engine.running())
  {
    engine.update();
    engine.render();    
  }
    
  engine.shutDown();

  if(scene != nullptr)
    delete scene;  


  return 0;  
}
