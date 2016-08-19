#include "Mesh.h"

#include "Engine.h"
#include "Log.h"
#include "BoundingBox.h"

#include <iostream>

Mesh::Mesh(const aiMesh*mesh, int mat_index)
{
  m_mat_index = mat_index;  

  aiVector3D zero(0.0f, 0.0f, 0.0f);

  float min_x = 0;
  float max_x = 0;
  float min_y = 0;
  float max_y = 0;
  float min_z = 0;
  float max_z = 0;
  bool f = true;

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
    m_indexed_representation.m_positions.push_back(glm::vec3(pPos->x,pPos->y,pPos->z )  );       
    m_indexed_representation.m_texCoords.push_back(glm::vec2(pTexCoord->x,1-pTexCoord->y)); // Flipp tex coords in y
    m_indexed_representation.m_normals.push_back(glm::vec3(pNormal->x,pNormal->y,pNormal->z ));
  }
  m_bounding_box = new BoundingBox(min_x,max_x,min_y,max_y,min_z,max_z);

  for (unsigned int i = 0 ; i < mesh->mNumFaces ; i++)
  {
    const aiFace& face = mesh->mFaces[i];
    assert(face.mNumIndices == 3);

    if(face.mNumIndices == 3)
    {
      m_indexed_representation.m_indices.push_back(face.mIndices[0]);
      m_indexed_representation.m_indices.push_back(face.mIndices[1]);
      m_indexed_representation.m_indices.push_back(face.mIndices[2]);

    }
  }
  initMesh(m_indexed_representation);
}

Mesh::Mesh(const  IndexedRepresentation& model, int mat_index)
{  
  m_indexed_representation = model;
  m_mat_index = mat_index;
  initMesh(m_indexed_representation);
}

Mesh::~Mesh()
{
    gl::DeleteVertexArrays(1, &m_vertex_array_object);
  if(m_bounding_box != nullptr)
    delete m_bounding_box;
}


// initialization -------------------------------------------------

void  Mesh::initMesh(const  IndexedRepresentation& model)
{
  m_draw_count = model.m_indices.size();
  gl::GenVertexArrays(1, &m_vertex_array_object);
  gl::BindVertexArray(m_vertex_array_object);

  gl::GenBuffers(NUM_BUFFERS, m_vertex_array_buffers);
  gl::BindBuffer(gl::ARRAY_BUFFER, m_vertex_array_buffers[POSITION_VB]);
  gl::BufferData(gl::ARRAY_BUFFER, model.m_positions.size() * sizeof(model.m_positions[0]), &model.m_positions[0], gl::STATIC_DRAW );

  gl::EnableVertexAttribArray(0);
  gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, 0, 0);

  gl::BindBuffer(gl::ARRAY_BUFFER, m_vertex_array_buffers[TEXCOORD_VB]);
  gl::BufferData(gl::ARRAY_BUFFER, model.m_positions.size() * sizeof(model.m_texCoords[0]), &model.m_texCoords[0], gl::STATIC_DRAW );

  gl::EnableVertexAttribArray(1);
  gl::VertexAttribPointer(1, 2, gl::FLOAT, gl::FALSE_, 0, 0);

  gl::BindBuffer(gl::ARRAY_BUFFER, m_vertex_array_buffers[NORMAL_VB]);
  gl::BufferData(gl::ARRAY_BUFFER, model.m_normals.size() * sizeof(model.m_normals[0]), &model.m_normals[0], gl::STATIC_DRAW );

  gl::EnableVertexAttribArray(2);
  gl::VertexAttribPointer(2, 3, gl::FLOAT, gl::FALSE_, 0, 0);

  gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, m_vertex_array_buffers[INDEX_VB]);
  gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, model.m_indices.size() * sizeof(model.m_indices[0]), &model.m_indices[0], gl::STATIC_DRAW );
 
  gl::BindVertexArray(0);

}


// buffer/render -------------------------------------------------

void Mesh::bufferModelMatrices(std::vector<glm::mat4> * matrices)
{
  gl::BindVertexArray(m_vertex_array_object);
  gl::BindBuffer(gl::ARRAY_BUFFER, m_vertex_array_buffers[MODEL_VB]);
  gl::BufferData(gl::ARRAY_BUFFER, matrices->size() * sizeof(glm::mat4), &(*matrices)[0], gl::STATIC_DRAW );

  for (unsigned int i = 0; i < 4 ; i++)
  {
    gl::EnableVertexAttribArray(3 + i); // 3 because we skip the index buffer
    gl::VertexAttribPointer(3 + i, 4, gl::FLOAT, gl::FALSE_, sizeof((*matrices)[0]), (const GLvoid*)(sizeof(glm::vec4) * i ));
    gl::VertexAttribDivisor(3 + i, 1);
  }

  gl::BindVertexArray(0);
}

void Mesh::render(unsigned int num_instances)
{  
	gl::BindVertexArray(m_vertex_array_object);
  gl::DrawElementsInstanced(gl::TRIANGLES, m_draw_count, gl::UNSIGNED_INT,0, num_instances);
  gl::BindVertexArray(0);
}

void Mesh::render()
{
  gl::BindVertexArray(m_vertex_array_object);
  gl::DrawElements(gl::TRIANGLES, m_draw_count, gl::UNSIGNED_INT,0);
  gl::BindVertexArray(0);
}


// getter/setters -------------------------------------------------

int Mesh::getMaterialIndex()
{
return m_mat_index;
}

BoundingBox * Mesh::getBoundingBox()
{
  return m_bounding_box;
}

IndexedRepresentation * Mesh::getIndexedRepresentation()
{
  return & m_indexed_representation;
}