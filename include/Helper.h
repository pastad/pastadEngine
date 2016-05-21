#ifndef HELPER_H
#define HELPER_H

#include <assimp/scene.h>
#include <glm/glm.hpp>

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


  static float m_debug_float;

};

#endif