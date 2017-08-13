#include "Terrain.h"

#include "Mesh.h"
#include "IndexedRepresentation.h"
#include "Engine.h"
#include "Log.h"
#include "RenderShader.h"
#include "RenderBaseShader.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

Terrain::Terrain()
{  
  m_mesh = nullptr;
}

Terrain::~Terrain()
{
  if(m_mesh != nullptr)
    delete m_mesh;  
}


float Terrain::heightFunction(glm::vec2 pos)
{
  return 0;
}


void Terrain::generate()
{
  IndexedRepresentation model;

  Engine::getLog()->log("Terrain", "generating");
  
  m_subdivision_amount = 1000;
  m_terrain_size =1000;
  float step_size = (float)m_terrain_size / (float) m_subdivision_amount;

  for( unsigned int y =0; y< m_subdivision_amount ; y++)
  {
    for( unsigned int x = 0; x < m_subdivision_amount ; x++)
    {
      model.m_positions.push_back(glm::vec3(x *step_size, heightFunction(glm::vec2( x *step_size,y * step_size )  )  , y * step_size)  );
      model.m_texCoords.push_back(glm::vec2(0,0)); // Flipp tex coords in y
      model.m_normals.push_back(glm::vec3(0,1,0 ));
    }
  }

  for(unsigned int y =0; y< m_subdivision_amount-1 ; y++)
  {
    for(unsigned int x = 0; x < m_subdivision_amount -1; x++)
    {     
       
      int offset = y * m_subdivision_amount;
      int offset_next = (y+1) *m_subdivision_amount;
      model.m_indices.push_back( offset +  x   );
      model.m_indices.push_back( offset +  x + 1 );
      model.m_indices.push_back( offset_next + x );

      model.m_indices.push_back( offset_next + x );
      model.m_indices.push_back( offset      + x + 1 );
      model.m_indices.push_back( offset_next + x + 1 );
      
    }
  }
  m_model_matrix =  glm::translate(glm::vec3((int)m_terrain_size /2 *-1  , 0, (int)m_terrain_size /2 *-1   )  );



  m_mesh = new Mesh(model, 0);
  Engine::getLog()->log("Terrain", "initialized");
}

void Terrain::render(RenderBaseShader * render_shader)
{
  render_shader->use();
  render_shader->setNotInstanced( m_model_matrix);
  render_shader->setIndentifcation(0);
  m_mesh->render();
}
void Terrain::renderWithoutMaterial(RenderBaseShader * shadow_shader)
{
  shadow_shader->use();
  shadow_shader->setNotInstanced( m_model_matrix );
  shadow_shader->setIndentifcation(0);
  m_mesh->render();
}
