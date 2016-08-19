#include "Water.h"

#include "Wave.h"
#include "Mesh.h"
#include "RenderBaseShader.h"
#include "Log.h"
#include "Engine.h"
#include "IndexedRepresentation.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include <sstream>
#include <iostream>

Water::Water(glm::vec3 center , float size): m_center(center), m_size(size)
{	
}

Water::~Water()
{	
  // clean up the waves
  for(std::vector<Wave*>::iterator it = m_waves.begin(); it != m_waves.end(); )
  {
    delete (*it);
    m_waves.erase(it);
  }
}

bool Water::initialize()
{
  IndexedRepresentation model;

  Engine::getLog()->log("Water", "initializing");

  float subdivision_amount = 10;

  float step_size = (float)m_size / (float) subdivision_amount;

  m_step = step_size;

  for( int y =0; y< subdivision_amount ; y++)
  {
    for( int x = 0; x < subdivision_amount ; x++)
    {
      model.m_positions.push_back(glm::vec3(x *step_size, 0 , y * step_size)  );
      model.m_texCoords.push_back(glm::vec2(0,0)); // Flipp tex coords in y
      model.m_normals.push_back(glm::vec3(0,1,0 ));
    }
  }

  for( int y =0; y< subdivision_amount-1 ; y++)
  {
    for( int x = 0; x < subdivision_amount -1; x++)
    {     
       
      int offset = y * subdivision_amount;
      int offset_next = (y+1) *subdivision_amount;
      model.m_indices.push_back( offset +  x   );
      model.m_indices.push_back( offset +  x + 1 );
      model.m_indices.push_back( offset_next + x );

      model.m_indices.push_back( offset_next + x );
      model.m_indices.push_back( offset      + x + 1 );
      model.m_indices.push_back( offset_next + x + 1 );
      
    }
  }
  m_model_matrix =  glm::translate(m_center +glm::vec3((int)m_size /2 *-1  , 0, (int)m_size /2 *-1   )  );

  m_mesh = new Mesh(model, 0);
  Engine::getLog()->log("Water", "initialized");


  return true;
}

void Water::render(RenderBaseShader * render_shader)
{
  gl::Enable (gl::BLEND);
  gl::BlendFunc (gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

  render_shader->use();
  setWavesForRender(render_shader);
  
  render_shader->setNotInstanced( m_model_matrix);
  render_shader->setIndentifcation(0);
  render_shader->setUniform("Step", m_step);
  m_mesh->render();

  gl::Disable (gl::BLEND);
}

float Water::getStep()
{
  return m_step;
}

Wave * Water::addWave(glm::vec2 direction, float length, float speed, float amplitude)
{
  Wave * wave =  new Wave(direction,length,speed,amplitude);
  m_waves.push_back(wave);

  return wave;
}

void Water::setWavesForRender(RenderBaseShader * render_shader)
{

  render_shader->setUniform("WaveAmount", (int)m_waves.size());
  std::stringstream ss, ssi;

  int count =0;
  for(std::vector<Wave*>::iterator it = m_waves.begin(); it != m_waves.end();it++ )
  {

    Wave * wave = (*it); 
    ss.str(""); ss.clear();
    ss << "Waves["<<count<<"]";

    ssi << ss.str() <<".Direction";
    render_shader->setUniform(ssi.str(),  wave->getDirection());
    ssi.str(""); ssi.clear();


    ssi << ss.str() <<".Amplitude";
    render_shader->setUniform(ssi.str(),  wave->getAmplitude());
    ssi.str(""); ssi.clear();

    ssi << ss.str() <<".Frequency";
    render_shader->setUniform(ssi.str(),  wave->getFrequency());
    ssi.str(""); ssi.clear();

    ssi << ss.str() <<".PhaseConstant";
    render_shader->setUniform(ssi.str(),  wave->getPhaseConstant());
    ssi.str(""); ssi.clear();

    count++;
  }
}