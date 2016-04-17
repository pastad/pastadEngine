#ifndef QUAD_H
#define QUAD_H


#include "gl_core_4_.h"
#include <glm/glm.hpp>

class Quad
{
public: 
  Quad();
  ~Quad();

  Quad(const Quad& other) = delete;
  Quad& operator=(const Quad& other) = delete;

  // loads the quad .. returns true if succesful
  bool load();

  // renders the quad
  void render();

private:

  // the handle for the buffer
  GLuint m_buffer_handle;

};

#endif // QUAD_H