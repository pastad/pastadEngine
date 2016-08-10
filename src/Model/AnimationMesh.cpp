#include "AnimationMesh.h"

#include "Helper.h"

#include "RenderShader.h"
#include "RenderBaseShader.h"
#include "BoundingBox.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

AnimationMesh::AnimationMesh(const aiMesh*mesh, int mat_index)
{  
  m_mat_index = mat_index;
  m_num_bones = mesh->mNumBones;

  IndexedRepresentation model;

  float min_x = 0;
  float max_x = 0;
  float min_y = 0;
  float max_y = 0;
  float min_z = 0;
  float max_z = 0;
  bool f = true;

  aiVector3D zero(0.0f, 0.0f, 0.0f);

  for(unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    aiVector3D* pPos      = &(mesh->mVertices[i]);
    aiVector3D* pNormal   = &(mesh->mNormals[i]);
    aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &zero;

    if( f|| (min_x > pPos->x) ) 
      min_x = pPos->x;
    if( f || (min_y > pPos->y) ) 
      min_y = pPos->y;
    if( f || (min_z > pPos->z) ) 
      min_z = pPos->z;

    if( f || (max_x < pPos->x) ) 
      max_x = pPos->x;
    if( f || (max_y < pPos->y) ) 
      max_y = pPos->y;
    if( f || (max_z < pPos->z) ) 
      max_z = pPos->z;

    f = false;

    model.m_positions.push_back(glm::vec3(pPos->x,pPos->y,pPos->z )  );
    model.m_texCoords.push_back(glm::vec2(pTexCoord->x,1-pTexCoord->y)); // Flipp tex coords in y
    model.m_normals.push_back(glm::vec3(pNormal->x,pNormal->y,pNormal->z ));
  } 

  for (unsigned int i = 0 ; i < mesh->mNumFaces ; i++)
  {
    const aiFace& face = mesh->mFaces[i];
    assert(face.mNumIndices == 3);

    if(face.mNumIndices == 3)
    {
      model.m_indices.push_back(face.mIndices[0]);
      model.m_indices.push_back(face.mIndices[1]);
      model.m_indices.push_back(face.mIndices[2]);
    }
  }

  m_bounding_box = new BoundingBox(min_x,max_x,min_y,max_y,min_z,max_z);

  model.m_bones.resize(mesh->mNumVertices);
  m_bone_info.resize(mesh->mNumBones);

  //  for the bones
  for (unsigned int i = 0 ; i < mesh->mNumBones ; i++)
  {
    unsigned int idx = 0;

    std::string BoneName(mesh->mBones[i]->mName.data);

    if (m_bone_mapping.find(BoneName) == m_bone_mapping.end())
    {
      idx = m_bone_count;
      m_bone_count++;
      BoneInfo bi;
      m_bone_info[idx].m_offset = Helper::toGlm(mesh->mBones[i]->mOffsetMatrix);
      m_bone_mapping[BoneName] = idx;
    }
    else    
      idx =  m_bone_mapping[BoneName];
    

    for (unsigned int j = 0 ; j < mesh->mBones[i]->mNumWeights ; j++)
    {
      unsigned int VertexID = mesh->mBones[i]->mWeights[j].mVertexId;
      float Weight  = mesh->mBones[i]->mWeights[j].mWeight;
      model.m_bones[VertexID].addBoneData(idx, Weight);
    }
  }

  initMesh(model);  
}

AnimationMesh::~AnimationMesh()
{  
  glDeleteVertexArrays(1, &m_vertex_array_object);
}


// initialization -------------------------------------------------

void AnimationMesh::initMesh(const  IndexedRepresentation& model)
{
  m_draw_count = model.m_indices.size();

  glGenVertexArrays(1, &m_vertex_array_object);
  glBindVertexArray(m_vertex_array_object);

  glGenBuffers(NUM_BUFFERS, m_vertex_array_buffers);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[POSITION_VB]);
  glBufferData(GL_ARRAY_BUFFER, model.m_positions.size() * sizeof(model.m_positions[0]), &model.m_positions[0], GL_STATIC_DRAW );

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[TEXCOORD_VB]);
  glBufferData(GL_ARRAY_BUFFER, model.m_positions.size() * sizeof(model.m_texCoords[0]), &model.m_texCoords[0], GL_STATIC_DRAW );

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[NORMAL_VB]);
  glBufferData(GL_ARRAY_BUFFER, model.m_normals.size() * sizeof(model.m_normals[0]), &model.m_normals[0], GL_STATIC_DRAW );

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertex_array_buffers[INDEX_VB]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.m_indices.size() * sizeof(model.m_indices[0]), &model.m_indices[0], GL_STATIC_DRAW );

  // we skip attribute index 3 because this would be the instanced model mat in the shader

  glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[BONE_VB]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(model.m_bones[0]) * model.m_bones.size(), &model.m_bones[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(4);
  glVertexAttribIPointer(4, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

  glBindVertexArray(0);

}


// render/apply|update transform -------------------------------------------------

void AnimationMesh::render()
{
  glBindVertexArray(m_vertex_array_object);
  glDrawElements(GL_TRIANGLES, m_draw_count, GL_UNSIGNED_INT,0);
  glBindVertexArray(0);
}

void AnimationMesh::applyTransform(std::string NodeName,glm::mat4 & global_inv, glm::mat4 & trans)
{
  if (m_bone_mapping.find(NodeName) != m_bone_mapping.end())
  {
    int BoneIndex = m_bone_mapping[NodeName];
    glm::mat4 t =global_inv  *trans * m_bone_info[BoneIndex].m_offset  ; 
    m_bone_info[BoneIndex].m_transformation = t; 
  }
}
void AnimationMesh::updateTransforms(std::vector<glm::mat4> *  transforms,  RenderBaseShader *shader)
{
  std::vector<glm::mat4> ts;
  for (unsigned int i = 0 ; i < m_num_bones ; i++)
  {
    (*transforms)[i] = m_bone_info[i].m_transformation;
    ts.insert(ts.end(), m_bone_info[i].m_transformation );
  }
  shader->setBones(&ts);
}


// getter/setters -------------------------------------------------

BoundingBox * AnimationMesh::getBoundingBox()
{
  return m_bounding_box;
}

int AnimationMesh::getMaterialIndex()
{
  return m_mat_index;
}

unsigned int AnimationMesh::getNumBones()
{
  return m_num_bones;
}