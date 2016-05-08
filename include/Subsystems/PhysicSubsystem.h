#ifndef PHYSICSUBSYSTEM
#define PHYSICSUBSYSTEM

#include "Engine.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Subsystem.h"

#include <string>

// Subsytem for IO operations

class Scene;
class SceneTreeElement;

class PhysicSubsystem : public Subsystem
{
public:
  PhysicSubsystem();
  ~PhysicSubsystem();

  // starts up the render system
  bool startUp();

  // shuts down the render system
  bool shutDown();

  // returns true if everything is up and running
  bool systemCheck();


  // --- Additionals ---
  
  void updateScene(Scene * scene, float delta);


private:

  // traverses through the scene during update
  void traverseScene( SceneTreeElement * element, float delta);
  
  
};




#endif //PHYSICSUBSYSTEM