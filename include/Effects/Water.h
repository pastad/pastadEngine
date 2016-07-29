#ifndef WATER_H
#define WATER_H

#include <string>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>


class Mesh;
class RenderBaseShader;
class Wave;

// class to represent a patch of water

class Water
{
public:
  Water(glm::vec3 center , float size);
  ~Water();

  Water(const Water& other) = delete;
  Water& operator=(const Water& other) = delete;

  // initializes the water patch
  bool initialize();

  // renders the water
  void render(RenderBaseShader * render_shader);
    
  // returns the step between vertices
  float getStep();

  // adds a wave to the water
  Wave * addWave(glm::vec2 direction, float length, float speed, float amplitude);

private:

  // the center of the patch
  glm::vec3 m_center;

  // the size of the patch
  float m_size;

  // the step between vertices
  float m_step;

  // holds the mesh of the patch
  Mesh * m_mesh;

  // the model matrix of the patch
  glm::mat4 m_model_matrix;

  // the waves of the water
  std::vector<Wave *> m_waves;

  // sets the waves for rendering
  void setWavesForRender(RenderBaseShader * render_shader);
  
};

#endif // WATER_H