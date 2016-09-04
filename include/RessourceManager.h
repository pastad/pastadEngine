#ifndef RESSOURCEMANAGER
#define RESSOURCEMANAGER_H

#include <string>
#include <map>
#include <mutex>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

class Model;
class Object;
class Texture;
class Material;
class Scene;

// loads the ressources and stores them to prevent loading multiple times

class RessourceManager
{
public:
	RessourceManager();
	~RessourceManager();

	// returns a new Object for the user
	Object * getObject(std::string path, Scene * scene);

  // returns the requested texture (and loads it if not loaded)
  static Texture * loadTexture(std::string path);

  // returns the requested model (and loads it if not loaded)
  static Model * loadModel(std::string path, bool instanced);

  // returns the materiall (and loads it if not loaded)
  static Material * loadMaterial(std::string dir, aiMaterial* mat);
  //static Material * loadMaterial(std::string dir, std::string name)

private:

	// holds the loaded models
	static std::map<std::string, Model *> m_models;

  // holds the loaded textures
  static std::map<std::string, Texture *> m_textures;

   // holds the loaded materials
  static std::map<std::string, Material *> m_materials;

  // the lock for mt
  static std::mutex m_lock;
  

};

#endif