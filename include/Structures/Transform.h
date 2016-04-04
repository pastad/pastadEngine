#ifndef TRANSFORM
#define TRANSFORM_H

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>

// stores pos,rot,scale and returns matrices

class Transform
{
public:
	Transform();
	~Transform();

	// takes position , rotation and scale
	Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);	

	// returns the model matrix
	glm::mat4 getModelMatrix();

	// recalculates the ModelMatrix in case of changes
	void recalculateMatrix();	

	// setters for position, rotation and scale
	void setPosition(glm::vec3 p);
	void setRotation(glm::vec3 r);
	void setScale(glm::vec3 s);

private:

	// holds the position
	glm::vec3 m_position;

	// holds the rotation
	glm::vec3 m_rotation;

	// holds the scale
	glm::vec3 m_scale;	

	// holds the model matrix	
	glm::mat4 m_model_matrix;
};

#endif