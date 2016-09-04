#include "RessourceManager.h"

#include "Model.h"
#include "Object.h"
#include "Material.h"
#include "Texture.h"
#include "Engine.h"
#include "Log.h"

#include <iostream>


std::map<std::string, Model *> RessourceManager::m_models;
std::map<std::string, Texture *> RessourceManager::m_textures;
std::map<std::string, Material *> RessourceManager::m_materials;
std::mutex RessourceManager::m_lock;

RessourceManager::RessourceManager()
{
}

RessourceManager::~RessourceManager()
{	
	for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
		delete it->second;
	for(std::map<std::string,  Texture *>::iterator iterator = m_textures.begin(); iterator != m_textures.end(); iterator++)
		delete iterator->second;
  
}


// loading functions ------------------------------------------

Model * RessourceManager::loadModel(std::string path, bool instanced)
{	
	Model * model = nullptr;

  m_lock.lock();
  Engine::getLog()->log("RessourceManager", "loading model", path);

	std::map<std::string, Model *>::iterator it = m_models.find(path);	

	if( it != m_models.end() )
	{
		model = it->second;
		Engine::getLog()->log("RessourceManager","found model",path);
	}
	else
	{
		model =  new Model(path, instanced);
		bool success  = model->load();
		if (success)
		{
			m_models[path] = model;
			Engine::getLog()->log("RessourceManager", "loaded model", path);
		}
		else
		{
			Engine::getLog()->log("RessourceManager","loading failed ",path);
			delete model;
			model = nullptr;
		}
	}
  m_lock.unlock();
  Engine::getLog()->log("RessourceManager", "done");
	return model;
}

Texture * RessourceManager::loadTexture(std::string path)
{	
	Texture * texture;

	std::map<std::string, Texture *>::iterator it = m_textures.find(path);
	
	if( it != m_textures.end() )
	{
		texture = it->second;
		Engine::getLog()->log("RessourceManager","found texture",path);
	}
	else
	{
		texture =  new Texture(path);
		if (texture->load())
		{
			m_textures[path] = texture;
			Engine::getLog()->log("RessourceManager", "loaded texture", path);
		}
		else
			Engine::getLog()->log("RessourceManager","failed loading",path);
		
		
	}

	return texture;
}

Material * RessourceManager::loadMaterial(std::string dir, aiMaterial* mat)
{	
	Material * material;
	aiString name;
  mat->Get(AI_MATKEY_NAME,name);
  std::string n(name.C_Str());

	std::map<std::string, Material *>::iterator it = m_materials.find(n);
	
	if( it != m_materials.end() )
	{
		material = it->second;
	}
	else
	{
		material =  new Material(mat,dir);
		m_materials[n] = material;
	}

	return material;
}
/*Material * RessourceManager::loadMaterial(std::string dir, std::string name)
{	
	Material * material;

	std::map<std::string, Material *>::iterator it = m_materials.find(name);
	
	if( it != m_materials.end() )
	{
		material = it->second;
	}
	else
	{
		material =  new Material(name,dir);
		m_materials[n] = material;
	}

	return material;
}*/
