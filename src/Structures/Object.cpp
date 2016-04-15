#include "Object.h"

Object::Object():m_transform(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0.5,0.5,0.5))
{	
}

Object::~Object()
{	
}

glm::mat4 Object::getModelMatrix()
{
	return m_transform.getModelMatrix();
}