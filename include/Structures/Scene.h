#ifndef SCENE
#define SCENE_H

// the scene class for objects,.. that should be rendered

#include <vector>
#include <string>
#include <map>

#include <glm/glm.hpp>

class Model;
class Object;
class Camera;
class RenderShader;
class RenderBaseShader;
class Light;
class Skybox;
class SkyboxShader;
class SceneTreeElement;
class Terrain;

class Scene
{
public:
	Scene();
	~Scene();

  Scene(const Scene& other) = delete;
  Scene& operator=(const Scene& other) = delete;


  // update function | requires the time difference
  void update(float delta);

  //renders the scene
	void render(RenderShader * render_shader, SkyboxShader * skybox_shader, RenderBaseShader * terrain_shader);

  // renders the scene for the shadow passes
  void renderShadow(RenderBaseShader * shadow_shader, RenderBaseShader* point_shadow_shader);

  // renders the skybox of the scene
  void renderSkybox(SkyboxShader * sybox_shader);

  // sets the skybox for the scene  returns | true if succeeded
  bool setSkybox(const std::string path);

  // sets the shadow textures
  void setupLightsForShadingPass(RenderShader * render_shader);

  // function to be called when camera is moved
  void cameraMoved();

  // function to be called when camera is rotated
  void cameraRotated();

  // refreshes all lights
  void refreshLights();

  // refrehses the visual objects
  void refreshRenderObjects(); 

  // returns the root scene tree element
  SceneTreeElement * getSceneRoot();


  // adds an object to the scene | single or instanced (with|without position)
  Object * addObject(std::string  path, glm::vec3 position);
  Object * addObjectInstanced(std::string path, glm::vec3 position);
  Object * addObject(std::string  path);
  Object * addObjectInstanced(std::string path);

  // adds a terrain to the scene
  Terrain * addTerrain();

  // adds a light to the scene
  Light * addLight();

  // returns the set camera
  Camera * getCamera();

  // retrieves the object with the id | or nullptr
  Object * getObject(int id);

  // removes the object from the scene | doesn't delete it
  void removeObject(Object * obj);

  // retrieves the light with the id | or nullptr
  Light * getLight(int id);

  // removes the light from the scene
  void removeLight(Light * l);

  // saves the scene to a path 
  void save(std::string path);

  // loads the scene specified by path
  bool load(std::string path);
  

private:

  // holds pointers to the objects
  std::vector<Object *> m_objects;

  // stores the models of the scene for rendering purposes
  std::map<std::string, Model *> m_models;

  // the objects to be rendered
  std::map<std::string, std::vector<Object *>> m_render_objects;

  // stores the lights of the scene
  std::vector<Light *> m_lights;

  // camera of the scene
  Camera * m_camera;

  // the skybox of the scene
  Skybox * m_skybox;

  // the root of the scene tree
  SceneTreeElement * m_tree_root = nullptr;

  // holds a terrain set for the scene
  Terrain * m_terrain;

  // number of objects in the scene for the ids
  int m_object_counter = 1000;

  // internal addObject request handling function ( with|without position)
  Object * addObject(std::string  path, glm::vec3 position, bool instanced);
  Object * addObject(std::string  path, bool instanced);

  // updates the animated objects
  void update(SceneTreeElement * element,  float delta);

  // returns a new object id
  int getObjectIdentification();

};

#endif