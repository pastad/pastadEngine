#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include <string>

#include "gl_core_4_.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture;

class RenderBuffer
{
public:
  RenderBuffer();
  ~RenderBuffer();

  RenderBuffer(const RenderBuffer& other) = delete;
  RenderBuffer& operator=(const RenderBuffer& other) = delete;

  // returns true if succeeded
  bool initialize();

  // bind and unbind
  void bindForInput();
  void unbindFromInput();
  void bindForOutput();


private:

  // the handle for the buffer
  GLuint m_buffer_handle;

  // the texture
  Texture *  m_buffer_texture;

};

#endif // RENDERBUFFER_H