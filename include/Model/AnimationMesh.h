#ifndef ANIMATIONMESH_H
#define ANIMATIONMESH_H

#include "Engine.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>



#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>

#include "IndexedRepresentation.h"

class RenderShader;
class RenderBaseShader;
class BoundingBox;


class AnimationMesh
{
public:
  AnimationMesh(const aiMesh*mesh, int mat_index);
  ~AnimationMesh();

  AnimationMesh(const AnimationMesh& other) = delete;
  AnimationMesh& operator=(const AnimationMesh& other)= delete ;

  // renders the mesh
  void render();

  // returns the material index
  int getMaterialIndex();

  // returns the number of bones
  unsigned int getNumBones();

  // retturns the global invers matrix
  glm::mat4 getGlobalInverse();

  // updates and applies the transforms
  void updateTransforms(std::vector<glm::mat4>* transforms,  RenderBaseShader *shader);
  void applyTransform(std::string NodeName,glm::mat4 & global_inv, glm::mat4 & trans);

    // returns the bb of the mesh
  BoundingBox * getBoundingBox();

protected:

private:

  // enum specifying the vb structures
  enum
  {
    POSITION_VB,
    TEXCOORD_VB,
    NORMAL_VB,
    INDEX_VB,
    TANGENT_VB,
    BITANGENT_VB,
    BONE_VB,
    NUM_BUFFERS
  };

  //  the vao and vbo
  GLuint m_vertex_array_object;
  GLuint m_vertex_array_buffers[NUM_BUFFERS];

  // holds the material index        
  int m_mat_index;

  // holds the draw count
  unsigned int m_draw_count = 0;

  // inits the mesh
  void initMesh(const  IndexedRepresentation& model);

  //  the bone count 
  unsigned int m_bone_count =0;
  unsigned int m_num_bones;

  // maps the bones to ids
  std::map<std::string,unsigned int> m_bone_mapping;

  // holds the info for the bones
  std::vector<BoneInfo> m_bone_info;

  // the global inverse matrix
  glm::mat4  m_global_inv;

  // the bounding box of the mesh
  BoundingBox * m_bounding_box;



};

#endif // ANIMATIONMESH_H
