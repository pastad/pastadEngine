#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction): m_origin(origin), m_direction(direction), m_bound_upper(-1.0f), m_bound_lower(-1.0f)
{	
	if(m_direction.x == -0.0f)
    m_direction.x =0.0f;
  if(m_direction.y == -0.0f)
    m_direction.y =0.0f;
  if(m_direction.z == -0.0f)
    m_direction.z =0.0f;
}

Ray::Ray(glm::vec3 origin, glm::vec3 direction, float lower, float upper): m_origin(origin), m_direction(direction), m_bound_upper(upper), m_bound_lower(lower)
{	
  if(m_direction.x == -0.0f)
    m_direction.x =0.0f;
  if(m_direction.y == -0.0f)
    m_direction.y =0.0f;
  if(m_direction.z == -0.0f)
    m_direction.z =0.0f;
}

Ray::~Ray()
{	
}


// getter/setters -------------------------------------------------

glm::vec3 Ray::getOrigin()
{
	return m_origin;
}
glm::vec3 Ray::getDirection()
{
	return m_direction;
}

float Ray::getLowerBound()
{
	return m_bound_lower;
}

float Ray::getUpperBound()
{
	return m_bound_upper;
}