#include "Helper.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <iostream>
#include <sstream>

#include "Engine.h"
#include "Log.h"

#include "tinyxml2.h"

float Helper::m_debug_float;

Helper::Helper()
{  
}

Helper::~Helper()
{  
}


// to glms ------------------------------------------

glm::mat4 Helper::toGlm(aiMatrix4x4 a)
{
  glm::mat4 m;
  glm::mat4 to;

  to[0][0] = (GLfloat)a.a1; to[1][0] = (GLfloat)a.a2;
  to[2][0] = (GLfloat)a.a3; to[3][0] = (GLfloat)a.a4;
  to[0][1] = (GLfloat)a.b1; to[1][1] = (GLfloat)a.b2;
  to[2][1] = (GLfloat)a.b3; to[3][1] = (GLfloat)a.b4;
  to[0][2] = (GLfloat)a.c1; to[1][2] = (GLfloat)a.c2;
  to[2][2] = (GLfloat)a.c3; to[3][2] = (GLfloat)a.c4;
  to[0][3] = (GLfloat)a.d1; to[1][3] = (GLfloat)a.d2;
  to[2][3] = (GLfloat)a.d3; to[3][3] = (GLfloat)a.d4;

  return to;
}

glm::mat4 Helper::toGlm(aiMatrix3x3 a)
{
  glm::mat4 m;  
  glm::mat4 to;

  to[0][0] = (GLfloat)a.a1; to[1][0] = (GLfloat)a.a2;
  to[2][0] = (GLfloat)a.a3; to[3][0] = (GLfloat)0;
  to[0][1] = (GLfloat)a.b1; to[1][1] = (GLfloat)a.b2;
  to[2][1] = (GLfloat)a.b3; to[3][1] = (GLfloat)0;
  to[0][2] = (GLfloat)a.c1; to[1][2] = (GLfloat)a.c2;
  to[2][2] = (GLfloat)a.c3; to[3][2] = (GLfloat)0;
  to[0][3] = (GLfloat)0; to[1][3] = (GLfloat)0;
  to[2][3] = (GLfloat)0; to[3][3] = (GLfloat)1;
  return to;
}



/*
glm::vec3 Helper::directionToAngle(glm::vec3 dir)
{
  glm::vec3 rot;
  float angle_x = glm::degrees( glm::angle(glm::vec3(0,1,0), glm::vec3(0,dir.y, dir.z) ) );

  float angle_y = glm::degrees( glm::angle(glm::vec3(0,0,1), glm::vec3(dir.x,0, dir.z) ) );

  float angle_z = glm::degrees( glm::angle(glm::vec3(0,1,0), glm::vec3(dir.x ,dir.y, 0) ) );

  rot = glm::vec3(angle_x,angle_y,angle_z);


  std::cout << angle_x <<","<< angle_y <<","<< angle_z<<std::endl;
  return rot;
}*/

// this and that ------------------------------------------

float Helper::gauss(float x, float sigma2 )
{
  double coeff = 1.0 / (glm::pi<double>() *2.0f * sigma2);
    double expon = -(x*x) / (2.0 * sigma2);
    return (float) (coeff*exp(expon));
} 

glm::vec3 Helper::anglesToDirection(float rot1, float rot2)
{
  glm::vec3 front;
  front.x = cos(glm::radians(rot1)) * cos(glm::radians(rot2));
  front.y = sin(glm::radians(rot2));
  front.z = sin(glm::radians(rot1)) * cos(glm::radians(rot2));
  return glm::normalize(front);
}

std::vector<std::string> Helper::split(const std::string &stri, char seperator) 
{
  std::vector<std::string> parts;
  std::stringstream ss(stri);
  std::string line;
  while (std::getline(ss, line, seperator))   
    parts.push_back(line);
  
  return parts;
}

GLfloat Helper::lerp(GLfloat a, GLfloat b, GLfloat c)
{
  return  c * (b -a) + a;
}  


// tinyxml read/write  ------------------------------------------

void Helper::insertToElement(tinyxml2::XMLElement * element, glm::vec3 v)
{
  element->SetAttribute("x", v.x);
  element->SetAttribute("y", v.y);
  element->SetAttribute("z", v.z);
}

void Helper::insertToElement(tinyxml2::XMLElement * element, glm::vec2 v)
{
  element->SetAttribute("x", v.x);
  element->SetAttribute("y", v.y);
}

void Helper::readFromElement(tinyxml2::XMLElement * element,glm::vec3 * v)
{
  element->QueryFloatAttribute("x", &v->x);
  element->QueryFloatAttribute("y", &v->y);
  element->QueryFloatAttribute("z", &v->z);
}

void Helper::readFromElement(tinyxml2::XMLElement * element,glm::vec2 * v)
{
  element->QueryFloatAttribute("x", &v->x);
  element->QueryFloatAttribute("y", &v->y);
}

void Helper::checkGLError(std::string who )
{
  GLenum error = gl::GetError();
  while (error != gl::NO_ERROR_)
  {
    if (error == gl::INVALID_ENUM)    
      Engine::getLog()->log(who, "invalid enum");
    if (error == gl::INVALID_VALUE)
      Engine::getLog()->log(who, "invalid value");
    if (error == gl::INVALID_OPERATION)
      Engine::getLog()->log(who, "invalid operation");
    if (error == gl::INVALID_FRAMEBUFFER_OPERATION)
      Engine::getLog()->log(who, "invalid framebuffer op");
    if (error == gl::OUT_OF_MEMORY)
      Engine::getLog()->log(who, "oom");      
    error = gl::GetError();
  }
}