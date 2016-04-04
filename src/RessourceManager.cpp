#include "RessourceManager.h"

#include "Model.h"


std::map<std::string, Model *> RessourceManager::m_models;

RessourceManager::RessourceManager()
{
}

RessourceManager::~RessourceManager()
{	
	for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
		delete it->second;
}

Model * RessourceManager::loadModel(std::string path)
{	
	Model * model;

	std::map<std::string, Model *>::iterator it = m_models.find(path);
	
	if( it != m_models.end() )
	{
		model = it->second;
	}
	else
	{
		model =  new Model();
		m_models[path] = model;
	}

	return model;
}