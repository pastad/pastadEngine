#ifndef SCENE
#define SCENE_H

// the scene class for objects,.. that should be rendered

#include <vector>
#include <string>
#include <map>
#include <mutex>

#define GLM_FORCE_RADIANS
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
class Water;

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
	void render(RenderShader * render_shader, SkyboxShader * skybox_shader, RenderBaseShader * terrain_shader, RenderBaseShader * water_shader, bool transparent);

  // renders the scene for the shadow passes
  void renderShadow(RenderBaseShader * shadow_shader, RenderBaseShader* point_shadow_shader, bool directional_enabled, bool point_enabled);

  // renders the skybox of the scene
  void renderSkybox(SkyboxShader * sybox_shader);

  // sets the skybox for the scene  returns | Skybox * or nullptr if failed
  Skybox * setSkybox(const std::string path);

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

  //  refrehses the visual objects using the scene tree
  void refreshRenderObjectsSceneTree();

  // returns the root scene tree element
  SceneTreeElement * getSceneRoot();


  // adds an object to the scene | single or instanced (with|without position)
  Object * addObject(std::string  path, glm::vec3 position, bool static_object);
  Object * addObjectInstanced(std::string path, glm::vec3 position, bool static_object);
  Object * addObject(std::string  path, bool static_object);
  Object * addObjectInstanced(std::string path, bool static_object);

  // adds an object to the scripted ones  | should be called by object
  void objectIsScripted(Object * obj);

  // adds a water effect to the scene
  Water * addWaterEffect(glm::vec3 pos, float size);

  // adds a terrain to the scene
  Terrain * addTerrain();

  // adds a light to the scene
  Light * addLight();

  // returns the set camera
  Camera * getCamera();

  // retrieves the object with the id | or nullptr
  Object * getObject(int id);

  // retrieves a vector with objects having ident as identifier
  std::vector<Object *> getObjects(std::string ident);

  // returns all objects
  std::vector<Object *> getObjects();

  // returns objects which are physics objects that are static
  std::vector<Object *> getPhysicsStaticObjects();

  // removes the object from the scene | doesn't delete it
  void removeObject(Object * obj);

  // retrieves the light with the id | or nullptr
  Light * getLight(int id);

  // retrieves all lights from the scene
  std::vector<Light *> getLights();

  // removes the light from the scene
  void removeLight(Light * l);

  // sets the fog
  void setFog(glm::vec3 color, float factor, float offset);

  // saves the scene to a path 
  void save(std::string path);

  // loads the scene specified by path
  bool load(std::string path);
    
  // returns/sets the time
  float getTime();
  void setTime(float time);

  // sets how fast the scene plays
  void setTimeAdvance(float speed);

  // returns the time advance speed
  float getTimeAdvance();

  // acquires the mutex lock
  void acquireLock(std::string who);
  void releaseLock(std::string who);

private:

  // holds pointers to the objects
  std::vector<Object *> m_objects_static;
  std::vector<Object *> m_objects_dynamic;

  std::vector<Model * >m_models_instanced;
  
  // holds pointers to objects that need refreshing every update
  std::vector<Object *> m_objects_animated;
  std::vector<Object *> m_objects_scripted;

  // stores the models of the scene for rendering purposes
  std::map<std::string, Model *> m_models;

  // the objects to be rendered
  std::map<std::string, std::vector<Object *>> m_render_objects;

  // stores the lights of the scene
  std::vector<Light *> m_lights;

  // stores the water effects
  std::vector<Water *> m_water_effects;

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

  // the current scene time and the time advance
  float m_time_line_seconds = 0.0f;
  float m_time_advance = 1.0f;

  // the fog specs
  glm::vec3 m_fog_color;
  float m_fog_factor;
  float m_fog_offset;

  // the mutex for thread safety
  std::mutex m_mutex;

  // internal addObject request handling function ( with|without position)
  Object * addObject(std::string  path, glm::vec3 position, bool instanced, bool insert_in_tree, bool static_object);
  Object * addObject(std::string  path, bool instanced, bool static_object);

  // updates the animated objects
  void update(SceneTreeElement * element,  float delta);

  // returns a new object id
  int getObjectIdentification();

  // updates the time by delta
  void timeUpdate(float delta);

  // returns a string of the current scene time
  std::string getTimeString();

  // the update functions
  static void updateObjectsScripts(std::vector<Object *> objs,  float delta, Scene * current);
  static void updateObjectsAnimated(std::vector<Object *> objs, float delta );


};

#endif
