#ifndef OBJECT
#define OBJECT_H

#include "Transform.h"

// the representation of an Model to the user


class Object : public Transform
{
public:
	Object();
	~Object();

	// returns the model matrix
	glm::mat4 getModelMatrix();

  using  Transform::setPosition;
  using  Transform::setRotation;
  using  Transform::setScale;

private:

	
};

#endif