#include "RessourceManager.h"

#include "Model.h"
#include "Object.h"
#include "Texture.h"
#include "Engine.h"
#include "Log.h"


std::map<std::string, Model *> RessourceManager::m_models;
std::map<std::string, Texture *> RessourceManager::m_textures;

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
