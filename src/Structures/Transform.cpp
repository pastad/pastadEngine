#include "Transform.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

Transform::Transform()
{	
	recalculateMatrix();
}
Transform::~Transform()
{	
}

Transform::Transform(glm::vec3 p,glm::vec3 r,glm::vec3 s ):m_position(p),m_rotation(r),m_scale(s)
{
	recalculateMatrix();
}

glm::mat4 Transform::getModelMatrix()
{
	return m_model_matrix;
}

void Transform::recalculateMatrix()
{
	glm::mat4 mat_trans =  glm::translate(m_position);

	glm::quat rot(m_rotation);
	glm::mat4 mat_rot = glm::mat4_cast(rot);

	glm::mat4 mat_scale = glm::scale(m_scale);

	m_model_matrix = mat_trans * mat_rot * mat_scale;
}

void Transform::setPosition(glm::vec3 p)
{
	m_position = p;
	recalculateMatrix();
}
void Transform::setRotation(glm::vec3 r)
{
	r = glm::vec3( glm::radians(r.x), glm::radians(r.y) , glm::radians(r.z) );
	m_rotation = r;
	recalculateMatrix();
}
void Transform::setScale(glm::vec3 s)
{
	m_scale = s;
	recalculateMatrix();
}