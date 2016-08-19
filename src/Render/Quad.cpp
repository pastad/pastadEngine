#include "Quad.h"

#include "Engine.h"
#include "Log.h"

Quad::Quad()
{
}

Quad::~Quad()
{  
}

//  load/render -------------------------------------------------
bool Quad::load()
{
  GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
  };
  GLfloat textue_coordinates[] = {
      0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
  };


  unsigned int handle[2];
  gl::GenBuffers(2, handle);

  gl::BindBuffer(gl::ARRAY_BUFFER, handle[0]);
  gl::BufferData(gl::ARRAY_BUFFER, 6 * 3 * sizeof(float), vertices, gl::STATIC_DRAW);

  gl::BindBuffer(gl::ARRAY_BUFFER, handle[1]);
  gl::BufferData(gl::ARRAY_BUFFER, 6 * 2 * sizeof(float), textue_coordinates, gl::STATIC_DRAW);
  
  gl::GenVertexArrays( 1, &m_buffer_handle );
  gl::BindVertexArray(m_buffer_handle);

  gl::BindBuffer(gl::ARRAY_BUFFER, handle[0]);
  gl::VertexAttribPointer( (GLuint)0, 3, gl::FLOAT, gl::FALSE_, 0, ((GLubyte *)NULL + (0)) );
  gl::EnableVertexAttribArray(0);

  gl::BindBuffer(gl::ARRAY_BUFFER, handle[1]);
  gl::VertexAttribPointer( (GLuint)1, 2, gl::FLOAT, gl::FALSE_, 0, ((GLubyte *)NULL + (0)) );
  gl::EnableVertexAttribArray(1);

  gl::BindVertexArray(0);

  return true;

}

void Quad::render()
{
  gl::BindVertexArray(m_buffer_handle);
  gl::DrawArrays(gl::TRIANGLES, 0, 6);
}