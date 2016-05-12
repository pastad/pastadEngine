#ifndef TERRAIN_H
#define TERRAIN_H

#include <assimp/scene.h>
#include <glm/glm.hpp>

class Terrain
{
public:
  Terrain();
  ~Terrain();

  Terrain(const Terrain& other) = delete;
  Terrain& operator=(const Terrain& other) = delete;

  // generates the terrain
  void generate();

  // renders the terrain  
  void render();

private:


};

#endif // TERRAIN_H