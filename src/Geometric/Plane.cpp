#include "Plane.h"

#include <iostream>

Plane::Plane()
{}

Plane::Plane(glm::vec3 p1,glm::vec3 p2, glm::vec3 p3): m_p1(p1),m_p2(p2),m_p3(p3)
{  
  glm::vec3 c1, c2;

  c1 = p1 - p2;
  c2 = p3 - p2;

  m_normal = glm::normalize( glm::cross(c2 , c1) );

  m_p = p2;
  m_d = -(glm::dot(m_normal,m_p));
}
Plane::~Plane()
{}


// getter/setters -------------------------------------------------

void Plane::setPoints(glm::vec3 p1,glm::vec3 p2, glm::vec3 p3)
{    
  m_p1 =p1;
  m_p2 =p2;
  m_p3 =p3;

  glm::vec3 c1 = p1 - p2;
  glm::vec3 c2 = p3 - p2;

  m_normal =  glm::cross(c2 , c1) ;
  if( glm::distance(m_normal, glm::vec3(0,0,0))  )
    m_normal = glm::normalize(m_normal);

  m_p = p2;

  m_d = -(glm::dot(m_normal,m_p));
 // std::cout <<m_d  <<std::endl;
 // std::cout <<c2.x<<","<<c2.y<<","<<c2.z<<std::endl;
}
float Plane::getDistance(glm::vec3 pos)
{ 
  return (m_d + glm::dot(m_normal,pos));
}