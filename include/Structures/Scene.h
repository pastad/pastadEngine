#ifndef SCENE
#define SCENE_H

// the scene class for objects,.. that should be rendered

#include <vector>
#include <map>

class Model;
class Object;
class Camera;
class RenderShader;
class Light;

class Scene
{
public:
	Scene();
	~Scene();

  // update function | requires the time difference
  void update(float delta);

  //renders the scene
	void render(RenderShader * render_shader);

  // adds an object to the scene
  Object * addObject(std::string  path);

  // adds a light to the scene
  Light * addLight();

  // returns the set camera
  Camera * getCamera();

private:

  // holds pointers to the objects
  std::vector<Object *> m_objects;

  // stores the models of the scene for rendering purposes
  std::map<std::string, Model *> m_models;

  // stores the lights of the scene
  std::vector<Light *> m_lights;

  // camera of the scene
  Camera * m_camera;
};

#endif