//#define NDEBUG

#include <stdio.h>
#include <iostream>
#include "Engine.h"

#include "Scene.h"
#include "Object.h"
#include "Script.h"
#include "RotationScriptElement.h"
#include "Light.h"
#include "IOSubsystem.h"

#include "Helper.h"

#include "Editor.h"

#include "GUI.h"
#include "Text.h"
#include "Image.h"
#include "Button.h"
#include <glm/glm.hpp>
#include <future>

#include <QtWidgets/QApplication>

static void startEditor(int argc, char *argv[])
{
  QApplication app(argc, argv);
  Editor w;
  w.show();

  app.exec();
}

int main(int argc, char *argv[])
{
  Engine engine;

  // intit the engine
	engine.initialize(1240, 720, RENDER_SUBSYSTEM , true, false);

  // load and set the start scene
	Scene *  scene = new Scene();
	scene->load("island-scene.xml");

  Object * obj = scene->addObject( "models/energy_remain.obj",glm::vec3(0,0,0) , false);
  if(obj != nullptr)
  {
    Script * s1 = obj->addScript();
    RotationScriptElement * rs1= s1->addRotationScript();
    rs1->setupAlways();
    rs1->setup(glm::vec3(100,0,0) ); 
  }

  Object * obj2 = scene->addObject("models/rooster.dae", glm::vec3(1, 0, 0), false);
  if (obj2 != nullptr)
  {
    obj2->setScale(glm::vec3(0.1f,0.1f,0.1f));
    Script * s2 = obj2->addScript();
    RotationScriptElement * rs2 = s2->addRotationScript();
    rs2->setupAlways();
    rs2->setup(glm::vec3(0,100,0));
  }

	engine.setScene(scene, false);

  std::future<void> future_editor = std::async(startEditor, argc, argv);
  

  // run the main loop
  while(engine.running())
  {
    engine.update();
    engine.render();    
  }
   
   // shut everything down
  engine.shutDown();

  future_editor.get();

  if(scene != nullptr)
    delete scene;  


  return 0;  
}
