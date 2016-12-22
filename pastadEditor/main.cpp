#include "PastadEditor.h"
#include <QtWidgets/QApplication>

#include <iostream>

#include "Engine.h"
#include "Scene.h"
#include "Camera.h"
#include "IOSubsystem.h"

PastadEditor *  w;
Engine engine;


bool startEngine(int argc, char *argv[])
{
  QApplication a(argc, argv);  
  w = new PastadEditor();
  engine.setPastadEditor(w);
  w->show();
  a.exec();
  Engine::stopRunning();
  return true;
}




int main(int argc, char *argv[])
{
 //   QApplication a(argc, argv);
  //  PastadEditor w;   


    
    if( !engine.initialize(1240, 720, RENDER_SUBSYSTEM | PHYSIC_SUBSYSTEM , 0, false) )
      return -1;

    engine.setShadowTechniqueDirectional(ST_STANDARD );
    engine.setShadowTechniquePoint(ST_STANDARD);

    engine.setPostProcessing(PP_FXAA, true);
    engine.setPostProcessing(PP_HDR, true);
    engine.setPostProcessing(PP_BLOOM, true);

  

    Scene * scene = new Scene();
    if( ! scene->load("sponza.xml") )
      return -1;
    engine.setScene(scene, false);
    scene->getCamera()->dontApplyPhysicsDrop();
    scene->getCamera()->unsetMovementCollisionCheck();
    scene->getCamera()->setRotationAllowed();

   // engine.run();
    IOSubsystem::registerMouseKeyCallback(&PastadEditor::mouseClicked);
    IOSubsystem::registerKeyCallback(&PastadEditor::keyPressed);
    std::future<bool> future = std::async(std::launch::async, startEngine,argc,argv);

    engine.run();
  //  w.show();
   // a.exec();
  //  engine.stopRunning();

    future.get();
    

    // shut everything down
    engine.shutDown();

 


    return 0;

}
