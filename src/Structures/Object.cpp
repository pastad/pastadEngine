#include "Object.h"

Object::Object():Transform(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(1,1,1))
{	
}

Object::~Object()
{	
}

glm::mat4 Object::getModelMatrix()
{
	return Transform::getModelMatrix();
}
