#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

// the class representing a ray in space

class Ray
{
public:
    Ray(glm::vec3 origin, glm::vec3 direction);
    Ray(glm::vec3 origin, glm::vec3 direction, float lower, float upper);
    ~Ray();

    Ray(const Ray& other) = delete;
    Ray& operator=(const Ray& other)= delete;

    // returns the origin of the ray
    glm::vec3 getOrigin();

    //returns the direction of the ray
	glm::vec3 getDirection();

	//returns lower bound or -1.0f if not set
	float getLowerBound();

	//returns upper bound or -1.0f if not set
	float getUpperBound();

private:

	// the origin of the ray
	glm::vec3 m_origin;

	// the direction of the ray
	glm::vec3 m_direction;

	// the bounds of the ray
	float m_bound_lower;	
	float m_bound_upper;

};

#endif //  RAY_H