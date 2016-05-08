#include "AnimationMesh.h"

#include "Helper.h"

#include "RenderShader.h"
#include "ShadowShader.h"
#include "PointShadowShader.h"

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

  aiVector3D zero(0.0f, 0.0f, 0.0f);

  for(unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    aiVector3D* pPos      = &(mesh->mVertices[i]);
    aiVector3D* pNormal   = &(mesh->mNormals[i]);
    aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &zero;

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

  model.m_bones.resize(mesh->mNumVertices);
  m_bone_info.resize(mesh->mNumBones);

  //  for the bones
  for (uint i = 0 ; i < mesh->mNumBones ; i++)
  {
    uint idx = 0;

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
    

    for (uint j = 0 ; j < mesh->mBones[i]->mNumWeights ; j++)
    {
      uint VertexID = mesh->mBones[i]->mWeights[j].mVertexId;
      float Weight  = mesh->mBones[i]->mWeights[j].mWeight;
      model.m_bones[VertexID].addBoneData(idx, Weight);
    }
    //std::cout <<i<<" Name:"<<std::string(mesh->mBones[i]->mName.data) <<" Weights: "<< mesh->mBones[i]->mNumWeights<<std::endl;
  }

  initMesh(model);  
}

AnimationMesh::~AnimationMesh()
{  
  glDeleteVertexArrays(1, &m_vertex_array_object);
}

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

void AnimationMesh::render()
{
  glBindVertexArray(m_vertex_array_object);
  glDrawElements(GL_TRIANGLES, m_draw_count, GL_UNSIGNED_INT,0);
  glBindVertexArray(0);
}

int AnimationMesh::getMaterialIndex()
{
  return m_mat_index;
}

unsigned int AnimationMesh::getNumBones()
{
  return m_num_bones;
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
void AnimationMesh::updateTransforms(std::vector<glm::mat4> *  transforms, RenderShader * render_shader, ShadowShader *ssshader, PointShadowShader * psshader )
{
  std::vector<glm::mat4> ts;
  for (uint i = 0 ; i < m_num_bones ; i++)
  {
    (*transforms)[i] = m_bone_info[i].m_transformation;
    ts.insert(ts.end(), m_bone_info[i].m_transformation );
  }
  if(render_shader != nullptr)
    render_shader->setBones(&ts);
  if(ssshader != nullptr)
    ssshader->setBones(&ts);
  if(psshader != nullptr)
    psshader->setBones(&ts);
}

