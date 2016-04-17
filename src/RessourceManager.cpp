#include "RessourceManager.h"

#include "Model.h"
#include "Object.h"
#include "Material.h"
#include "Texture.h"
#include "Engine.h"
#include "Log.h"


std::map<std::string, Model *> RessourceManager::m_models;
std::map<std::string, Texture *> RessourceManager::m_textures;
std::map<std::string, Material *> RessourceManager::m_materials;

RessourceManager::RessourceManager()
{
}

RessourceManager::~RessourceManager()
{	
	for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
		delete it->second;
	for(std::map<std::string,Texture *>::iterator iterator = m_textures.begin(); iterator != m_textures.end(); iterator++)
    delete iterator->second;
  
}

Model * RessourceManager::loadModel(std::string path)
{	
	Model * model;

	std::map<std::string, Model *>::iterator it = m_models.find(path);
	
	if( it != m_models.end() )
	{
		model = it->second;
		Engine::getLog()->log("RessourceManager","found model",path);
	}
	else
	{
		model =  new Model(path);
		model->load();
		m_models[path] = model;
		Engine::getLog()->log("RessourceManager","loaded model",path);
	}

	return model;
}

Texture * RessourceManager::loadTexture(std::string path)
{	
	Texture * texture;

	std::map<std::string, Texture *>::iterator it = m_textures.find(path);
	
	if( it != m_textures.end() )
	{
		texture = it->second;
	}
	else
	{
		texture =  new Texture(path);
		texture->load();
		m_textures[path] = texture;
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