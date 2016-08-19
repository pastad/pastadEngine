#ifndef PHYSICSUBSYSTEM
#define PHYSICSUBSYSTEM

#include "Engine.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Subsystem.h"

#include <string>

// Subsytem for physics,collision operations

class Scene;
class SceneTreeElement;

class Model;
class Mesh;
class Ray;
class BoundingBox;

class PhysicSubsystem : public Subsystem
{
public:
  PhysicSubsystem();
  ~PhysicSubsystem();

  // from base class
  using Subsystem::systemCheck;
  using Subsystem::startUp;
  using Subsystem::shutDown;



  // --- Additionals ---
  
  void updateScene(Scene * scene, float delta);

  // collison between ray and scene 
  bool collisionRayScene(Scene * scene, Ray * ray, float * distance);

  // collision between ray and object | distance is smallest distance with intersection(s)
  bool collisionRayObject(Object * object, Ray * ray, float * distance);

  // collision between ray and a axis aligned bounding box
  bool collisionRayAABB(Ray * ray, BoundingBox * aabb);

  // collision between ray and mesh
  bool collisionRayMesh( Mesh * m , glm::mat4 transform, Ray * ray ,float * distance);

  // collision between ray and triangle
  bool collisionRayTriangle( glm::vec3 p1, glm::vec3 p2 , glm::vec3 p3, Ray * ray, float * distance );


  // collision between an object and a list of objects | returns Object or nullptr
  Object *  collisionObjectObjects(std::vector<Object*> objs, Object * object);

  // collision between an object and an object
  bool  collisionObjectObject(Object * object1, Object * object2);


  bool collisionMovement(Scene * scene, glm::vec3 pos, glm::vec3 dir , float step, float surrounding_offset, float bottom_offset, glm::vec3 * new_pos);


private:

  // traverses through the scene during update
  void traverseScene( SceneTreeElement * element,Scene * scene, float delta);
  
  
};




#endif //PHYSICSUBSYSTEM