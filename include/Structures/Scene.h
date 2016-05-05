#ifndef SCENE
#define SCENE_H

// the scene class for objects,.. that should be rendered

#include <vector>
#include <string>
#include <map>

class Model;
class Object;
class Camera;
class RenderShader;
class ShadowShader;
class PointShadowShader;
class Light;
class Skybox;
class SkyboxShader;

class Scene
{
public:
	Scene();
	~Scene();

  // update function | requires the time difference
  void update(float delta);

  //renders the scene
	void render(RenderShader * render_shader, SkyboxShader * skybox_shader);

  // renders the scene for the shadow passes
  void renderShadow(ShadowShader * shadow_shader, PointShadowShader* point_shadow_shader);

  // renders the skybox of the scene
  void renderSkybox(SkyboxShader * sybox_shader);

  // sets the skybox for the scene  returns | true if succeeded
  bool setSkybox(const std::string path);

  // sets the shadow textures
  void setupLightsForShadingPass(RenderShader * render_shader);

  // function to be called when camera is moved
  void cameraMoved();

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

  // the skybox of the scene
  Skybox * m_skybox;

};

#endif