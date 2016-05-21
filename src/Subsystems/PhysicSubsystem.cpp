#include "PhysicSubsystem.h"

#include "Engine.h"
#include "Log.h"
#include "Scene.h"
#include "Camera.h"

#include "SceneTreeElement.h"
#include "Object.h"

#include <iostream>
#include <fstream>
#include <sstream>



PhysicSubsystem::PhysicSubsystem()
{
  m_initialized = false;
}

PhysicSubsystem::~PhysicSubsystem()
{
}


bool PhysicSubsystem::startUp()
{
  if(! m_initialized)
  {
    // init system here
    m_initialized =true;  
    Engine::getLog()->log("PhysicSubsystem", "started");
    return true;
  }
  return false;
}

bool PhysicSubsystem::shutDown()
{
  if(m_initialized)
  {

    m_initialized = false;
    Engine::getLog()->log("PhysicSubsystem", "shut down");
    return true;
  }
  return false;
}

bool PhysicSubsystem::systemCheck()
{
  return m_initialized;
}

void PhysicSubsystem::updateScene(Scene * scene, float delta)
{
  SceneTreeElement * root = scene->getSceneRoot();
  traverseScene(root, delta);
  //scene->cameraMoved();
}

void PhysicSubsystem::traverseScene( SceneTreeElement * element , float delta)
{
  std::vector< SceneTreeElement *> children = element->getChildren();
  std::vector< Object *> objects = element->getObjects();
  for( int x=0;  x< objects.size(); x++)
  {
    Object * obj = objects.at(x);
    //if(obj->isPhysicsApplied())
     // obj->move(glm::vec3(0,-0.1f,0) *delta   );    
  }


  for( int x=0;  x< children.size(); x++)
  {
    traverseScene(children.at(x), delta );
  }
}