#include "Quad.h"

#include "Engine.h"
#include "Log.h"

Quad::Quad()
{
}
Quad::~Quad()
{  
}

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
  glGenBuffers(2, handle);

  glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
  glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
  glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), textue_coordinates, GL_STATIC_DRAW);
  
  glGenVertexArrays( 1, &m_buffer_handle );
  glBindVertexArray(m_buffer_handle);

  glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
  glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
  glEnableVertexAttribArray(0); 

  glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
  glVertexAttribPointer( (GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
  glEnableVertexAttribArray(1);  

  glBindVertexArray(0);

  return true;

}

void Quad::render()
{
  glBindVertexArray(m_buffer_handle);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}