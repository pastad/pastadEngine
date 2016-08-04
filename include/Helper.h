#ifndef HELPER_H
#define HELPER_H

#include <assimp/scene.h>
#include "gl_core_4_.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "tinyxml2.h"


class Helper
{
public:
  Helper();
  ~Helper();

  Helper(const Helper& other) = delete;
  Helper& operator=(const Helper& other) = delete;

  // converts from assimp to glm
  static glm::mat4 toGlm(aiMatrix4x4 a);
  static glm::mat4 toGlm(aiMatrix3x3 a);

  // calculates the gauss values
  static float gauss(float x, float sigma);

  // converts angles to a direction vector
  static glm::vec3 anglesToDirection(float rot1, float rot2);

  // split string method
  static std::vector<std::string> split(const std::string &stri, char seperator);

  // inserts the vec3/vec2 in the element
  static void insertToElement(tinyxml2::XMLElement * element, glm::vec3 v);
  static void insertToElement(tinyxml2::XMLElement * element, glm::vec2 v);

  // reads the vec3/vec2 from the element
  static void readFromElement(tinyxml2::XMLElement * element,glm::vec3 * v);
  static void readFromElement(tinyxml2::XMLElement * element,glm::vec2 * v);

  // interolation function
  static GLfloat lerp(GLfloat a, GLfloat b, GLfloat c);

  static float m_debug_float;

};

#endif