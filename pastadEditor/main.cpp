#include "PastadEditor.h"
#include <QtWidgets/QApplication>

#include <iostream>

#include "Engine.h"
#include "IOSubsystem.h"



bool startEngine(int argc, char *argv[])
{
  QApplication a(argc, argv);  
  PastadEditor w;
  w.show();
  a.exec();
  Engine::stopRunning();
  return true;
}




int main(int argc, char *argv[])
{
 //   QApplication a(argc, argv);
  //  PastadEditor w;
   

    Engine engine;	   
    
    if( !engine.initialize(1680, 840, RENDER_SUBSYSTEM , 0, false) )
      return -1;

    engine.setPostProcessing(PP_FXAA, true);
    engine.setPostProcessing(PP_HDR, true);
    engine.setPostProcessing(PP_BLOOM, true);


    Scene * scene = new Scene();
    if( ! scene->load("island-scene.xml") )
      return -1;
    engine.setScene(scene, false);

   // engine.run();
    IOSubsystem::registerMouseKeyCallback(&PastadEditor::mouseClicked);
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
