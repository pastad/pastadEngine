#ifndef MESH_H
#define MESH_H

//#include "gl_core_4_.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Engine.h"
#include <glm/glm.hpp>
#include <vector>

#include "IndexedRepresentation.h"

class BoundingBox;

class Mesh
{
public:
  Mesh(const aiMesh*mesh, int mat_index);
  Mesh(const  IndexedRepresentation& model, int mat_index);
  ~Mesh();

  Mesh(const Mesh& other) = delete;
  Mesh& operator=(const Mesh& other)= delete ;

  // renders the mesh  | instanced or not
  void render(unsigned int num_indices) ;
  void render();

  // returns the material index
  int getMaterialIndex();

  // buffers the matrices to a gml buffer
  void bufferModelMatrices(std::vector<glm::mat4> * matrices);

  // returns the bb of the mesh
  BoundingBox * getBoundingBox();

  // returns the indexed representation
  IndexedRepresentation * getIndexedRepresentation();
  
protected:

private:

  // enum specifying the vb structures
  enum
  {
      POSITION_VB,
      TEXCOORD_VB,
      NORMAL_VB,
      INDEX_VB,
      MODEL_VB,
      NUM_BUFFERS
  };

  //  the vao and vbo
  GLuint m_vertex_array_object;
  GLuint m_vertex_array_buffers[NUM_BUFFERS];

  // holds the material index        
  int m_mat_index;

  // holds the draw count
  unsigned int m_draw_count = 0;

  // the bounding box of the mesh
  BoundingBox * m_bounding_box;

  // the indexed representation of the mesh
  IndexedRepresentation m_indexed_representation;

  // inits the mesh
  void initMesh(const  IndexedRepresentation& model);
};

#endif // MESH_H