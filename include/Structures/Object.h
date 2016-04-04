#ifndef OBJECT
#define OBJECT_H

#include "Transform.h"

// the representation of an Model to the user


class Object
{
public:
	Object();
	~Object();

	// returns the model matrix
	glm::mat4 getModelMatrix();

private:

	// holds the transform of the object
	Transform m_transform;
};

#endif