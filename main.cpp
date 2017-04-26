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


#include "Player.h"

#include "Helper.h"

#include "GUI.h"
#include "Text.h"
#include "Image.h"
#include "Button.h"
#include <glm/glm.hpp>
#include <future>
#include <windows.h>

#include "Camera.h"

#include <QtWidgets/QApplication>

/*static void startEditor(int argc, char *argv[])
{
  QApplication app(argc, argv);
  Editor w;
  w.show();

  app.exec();
}*/

int main(int argc, char *argv[])
{

  Engine engine;

  // intit the engine
	engine.initialize("engine_config.xml");//1680, 840, RENDER_SUBSYSTEM , 2, false);
  
  engine.setPostProcessing(PP_FXAA, true);
  engine.setPostProcessing(PP_HDR, true);
  engine.setPostProcessing(PP_BLOOM, true);

  // load and set the start scene
	Scene *  scene = new Scene();	

 


  Light * l = scene->addLight();
  l->setDirectional(glm::vec3(0, -1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 0.5f, true);

	engine.setScene(scene, false);

  Object * pobj = scene->addObject("game/low_poly.obj", false);
  pobj->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
  Player * player = new Player(pobj, scene->getCamera());
  pobj->applyPhysics();

 // Object * terrain = scene->addObject("game/terrain.obj", true);
//  terrain->applyPhysicsStatic();
 // terrain->applyPhysicsStatic();

  Cloth * cl = scene->addClothEffect(glm::vec3(0, 3, 0), 2);
  

  // run the main loop
  while(engine.running())
  {
    engine.update();
    engine.render();  
    player->update(0.01f);
  }
   
   // shut everything down
  engine.shutDown();

  //future_editor.get();

  if(scene != nullptr)
    delete scene;  

  delete player;

  return 0;  
}
