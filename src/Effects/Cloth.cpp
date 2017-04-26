#include "Cloth.h"

#include "Transform.h"
#include "ComputeShader.h"
#include "RenderShader.h"



#include <glm\glm.hpp>

#include <vector>

Cloth::Cloth()
{
  m_transform = new Transform();
}


Cloth::~Cloth()
{
  delete m_transform;
}

bool Cloth::init(float x , float y)
{
  float dx = x    / (CLOTH_PARTICLE_SIZE_X - 1);
  float dy = y    / (CLOTH_PARTICLE_SIZE_Y - 1);
  float ds = 1.0f / (CLOTH_PARTICLE_SIZE_X - 1);
  float dt = 1.0f / (CLOTH_PARTICLE_SIZE_Y - 1);

  std::vector<GLfloat> pos;
  std::vector<GLfloat> vel(CLOTH_PARTICLE_SIZE_X * CLOTH_PARTICLE_SIZE_Y * 4, 0.0f);
  std::vector<GLfloat> tex_coord;

  glm::vec4 point(0.0f, 0.0f, 0.0f, 1.0f);
  for (int i = 0; i < CLOTH_PARTICLE_SIZE_Y; i++)
  {
    for (int j = 0; j < CLOTH_PARTICLE_SIZE_X; j++)
    {
      point.x = dx * j;
      point.y = dy * i;
      point.z = 0.0f;

      point = m_transform->getModelMatrix() * point;
      pos.push_back(point.x);
      pos.push_back(point.y);
      pos.push_back(point.z);
     // pos.push_back(1.0f);

      tex_coord.push_back(ds * j);
      tex_coord.push_back(dt * i);
    }
  }
  std::vector<GLuint> el;

  for (int row = 0; row < CLOTH_PARTICLE_SIZE_Y - 1; row++)
  {

    for (int col = 0; col < CLOTH_PARTICLE_SIZE_X; col++)
    {
      el.push_back((row + 1) * CLOTH_PARTICLE_SIZE_X + (col));
      el.push_back((row)* CLOTH_PARTICLE_SIZE_X + (col));
    }
    el.push_back(PRIM_RESTART);
  }

  GLuint bufs[7];
  gl::GenBuffers(7, bufs);
  m_pos_buffer[0] = bufs[0];
  m_pos_buffer[1] = bufs[1];
  m_vel_buffer[0] = bufs[2];
  m_vel_buffer[1] = bufs[3];
  m_normal_buffer = bufs[4];
  m_el_buffer = bufs[5];
  m_tex_coord_buffer = bufs[6];

  GLuint parts = CLOTH_PARTICLE_SIZE_X * CLOTH_PARTICLE_SIZE_Y;

  gl::BindBufferBase(gl::SHADER_STORAGE_BUFFER, 0, m_pos_buffer[0]);
  gl::BufferData(gl::SHADER_STORAGE_BUFFER, parts * 3 * sizeof(GLfloat), &pos[0], gl::DYNAMIC_DRAW);
  gl::BindBufferBase(gl::SHADER_STORAGE_BUFFER, 1, m_pos_buffer[1]);
  gl::BufferData(gl::SHADER_STORAGE_BUFFER, parts * 3 * sizeof(GLfloat), NULL, gl::DYNAMIC_DRAW);

  gl::BindBufferBase(gl::SHADER_STORAGE_BUFFER, 2, m_vel_buffer[0]);
  gl::BufferData(gl::SHADER_STORAGE_BUFFER, parts * 3 * sizeof(GLfloat), &vel[0], gl::DYNAMIC_COPY);
  gl::BindBufferBase(gl::SHADER_STORAGE_BUFFER, 3, m_vel_buffer[1]);
  gl::BufferData(gl::SHADER_STORAGE_BUFFER, parts * 3 * sizeof(GLfloat), NULL, gl::DYNAMIC_COPY);

  gl::BindBufferBase(gl::SHADER_STORAGE_BUFFER, 4, m_normal_buffer);
  gl::BufferData(gl::SHADER_STORAGE_BUFFER, parts * 3 * sizeof(GLfloat), NULL, gl::DYNAMIC_COPY);

  gl::BindBuffer(gl::ARRAY_BUFFER, m_el_buffer);
  gl::BufferData(gl::ARRAY_BUFFER, el.size() * sizeof(GLuint), &el[0], gl::DYNAMIC_COPY);


  gl::BindBuffer(gl::ARRAY_BUFFER, m_tex_coord_buffer);
  gl::BufferData(gl::ARRAY_BUFFER, tex_coord.size() * sizeof(GLfloat), &tex_coord[0], gl::STATIC_DRAW);


  m_num_elements = GLuint(el.size());

  // vao
  gl::GenVertexArrays(1, &m_vao);
  gl::BindVertexArray(m_vao);

  gl::BindBuffer(gl::ARRAY_BUFFER, m_pos_buffer[0]);
  gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, 0, 0);
  gl::EnableVertexAttribArray(0);

  gl::BindBuffer(gl::ARRAY_BUFFER, m_normal_buffer);
  gl::VertexAttribPointer(1, 3, gl::FLOAT, gl::FALSE_, 0, 0);
  gl::EnableVertexAttribArray(2);

  gl::BindBuffer(gl::ARRAY_BUFFER, m_tex_coord_buffer);
  gl::VertexAttribPointer(2, 2, gl::FLOAT, gl::FALSE_, 0, 0);
  gl::EnableVertexAttribArray(1);

  gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, m_el_buffer);
  gl::BindVertexArray(0);


  return true;
}
void Cloth::compute( ComputeShader  *cs)
{

}

void Cloth::render(RenderShader * rs)
{
  rs->setNotInstanced(m_transform->getModelMatrix());
  gl::BindVertexArray(m_vao);
  gl::DrawElements(gl::TRIANGLE_STRIP, m_num_elements, gl::UNSIGNED_INT, 0);
  gl::BindVertexArray(0);
}