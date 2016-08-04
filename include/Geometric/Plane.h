#ifndef PLANE_H
#define PLANE_H

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// class that represents a plane | specified by three points

class Plane
{
public:
    Plane();
    Plane(glm::vec3 p1,glm::vec3 p2, glm::vec3 p3);
    ~Plane();

    Plane(const Plane& other) = delete;
    Plane& operator=(const Plane& other) = delete;

    // sets the points of the plane
    void setPoints(glm::vec3 p1,glm::vec3 p2, glm::vec3 p3);

    // returns the distance between pos and the plane
    float getDistance(glm::vec3 pos);

protected:  

    // points to form the plane
    glm::vec3 m_p1;
    glm::vec3 m_p2;
    glm::vec3 m_p3;

    // other representation
    glm::vec3 m_normal;
    glm::vec3 m_p;
    float m_d;

};
#endif