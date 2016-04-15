#ifndef SCENE
#define SCENE_H

// the scene class for objects,.. that should be rendered

#include <vector>
#include <map>

class Model;
class Object;
class Camera;

class Scene
{
public:
	Scene();
	~Scene();

  // update function
  void update();

  //renders the scene
	void render();

  // adds an object to the scene
  Object * addObject(std::string  path);

  // returns the set camera
  Camera * getCamera();

private:

  // holds pointers to the objects
  std::vector<Object *> m_objects;

  // stores the models of the scene for rendering purposes
  std::map<std::string, Model *> m_models;

  // camera of the scene
  Camera * m_camera;
};

#endif