#ifndef TERRAIN_H
#define TERRAIN_H

#include <assimp/scene.h>
#include <glm/glm.hpp>

class Mesh;
class RenderShader;
class RenderBaseShader;

class Terrain
{
public:
  Terrain();
  ~Terrain();

  Terrain(const Terrain& other) = delete;
  Terrain& operator=(const Terrain& other) = delete;

  // generates the terrain
  void generate();

  // the function that generates the terrain
  float heightFunction(glm::vec2 pos);

  // renders the terrain  
  void render(RenderBaseShader * render_shader);
  void renderWithoutMaterial(RenderBaseShader * shaow_shader);

private:

  // holds the mesh of the terrain
  Mesh * m_mesh;

  // holds the size of the terrain
  unsigned int m_terrain_size;

  // determines the smoothness of the terrain
  unsigned int m_subdivision_amount;

  // the model matrix of the terrain
  glm::mat4 m_model_matrix;

};

#endif // TERRAIN_H