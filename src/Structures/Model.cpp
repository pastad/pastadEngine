#include "Model.h"

#include "Object.h"


Model::Model(std::string path):m_path(path)
{	
}

Model::~Model()
{
	for(std::vector<Object *>::iterator it = m_instances.begin(); it != m_instances.end();it++)
		delete (*it);	
}

Object * Model::getInstance()
{
	Object * obj = new Object();
	m_instances.insert(m_instances.end(),obj);
	return obj;
}

void Model::render()
{	
}

bool Model::load()
{

	return true;
}