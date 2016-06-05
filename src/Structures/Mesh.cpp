#include "Mesh.h"

#include "IndexedRepresentation.h"
#include "Engine.h"
#include "Log.h"
#include "BoundingBox.h"

#include <iostream>

Mesh::Mesh(const aiMesh*mesh, int mat_index)
{
  m_mat_index = mat_index;
  IndexedRepresentation model;

  aiVector3D zero(0.0f, 0.0f, 0.0f);

  float min_x = -1;
  float max_x = -1;
  float min_y = -1;
  float max_y = -1;
  float min_z = -1;
  float max_z = -1;

  for(unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    aiVector3D* pPos      = &(mesh->mVertices[i]);
    aiVector3D* pNormal   = &(mesh->mNormals[i]);
    aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &zero;

    if( (min_x == -1) || (min_x > pPos->x) ) 
      min_x = pPos->x;
    if( (min_y == -1) || (min_y > pPos->y) ) 
      min_y = pPos->y;
    if( (min_z == -1) || (min_z > pPos->z) ) 
      min_z = pPos->z;

    if( (max_x == -1) || (max_x < pPos->x) ) 
      max_x = pPos->x;
    if( (max_y == -1) || (max_y < pPos->y) ) 
      max_y = pPos->y;
    if( (max_z == -1) || (max_z < pPos->z) ) 
      max_z = pPos->z;


    model.m_positions.push_back(glm::vec3(pPos->x,pPos->y,pPos->z )  );
    model.m_texCoords.push_back(glm::vec2(pTexCoord->x,1-pTexCoord->y)); // Flipp tex coords in y
    model.m_normals.push_back(glm::vec3(pNormal->x,pNormal->y,pNormal->z ));
  }
  m_bounding_box = new BoundingBox(min_x,min_y,min_z,max_x,max_y,max_z);

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

  initMesh(model);
}
Mesh::Mesh(const  IndexedRepresentation& model, int mat_index)
{
  m_mat_index = mat_index;
  initMesh(model);
}


Mesh::~Mesh()
{
  glDeleteVertexArrays(1, &m_vertex_array_object);
  if(m_bounding_box != nullptr)
    delete m_bounding_box;
}

void  Mesh::initMesh(const  IndexedRepresentation& model)
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

  glBindVertexArray(0);

}

void Mesh::bufferModelMatrices(std::vector<glm::mat4> * matrices)
{
  glBindVertexArray(m_vertex_array_object);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffers[MODEL_VB]);
  glBufferData(GL_ARRAY_BUFFER, matrices->size() * sizeof(glm::mat4), &(*matrices)[0], GL_STATIC_DRAW );

  for (unsigned int i = 0; i < 4 ; i++)
  {
    glEnableVertexAttribArray(3 + i); // 3 because we skip the index buffer
    glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof((*matrices)[0]), (const GLvoid*)(sizeof(glm::vec4) * i ));
    glVertexAttribDivisor(3 + i, 1);
  }

  glBindVertexArray(0);
}

void Mesh::render(unsigned int num_instances)
{  
  glBindVertexArray(m_vertex_array_object);
  glDrawElementsInstanced(GL_TRIANGLES, m_draw_count, GL_UNSIGNED_INT,0, num_instances);
  glBindVertexArray(0);
}

void Mesh::render()
{
  glBindVertexArray(m_vertex_array_object);
  glDrawElements(GL_TRIANGLES, m_draw_count, GL_UNSIGNED_INT,0);
  glBindVertexArray(0);
}

int Mesh::getMaterialIndex()
{
return m_mat_index;
}

BoundingBox * Mesh::getBoundingBox()
{
  return m_bounding_box;
}