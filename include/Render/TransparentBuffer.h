#ifndef TRANSPARENTBUFFER_H
#define TRANSPARENTBUFFER_H

#include <string>

#define GLM_FORCE_RADIANS
#include "gl_core_4_.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture;

class TransparentBuffer
{
public:
  TransparentBuffer();
  ~TransparentBuffer();

  TransparentBuffer(const TransparentBuffer& other) = delete;
  TransparentBuffer& operator=(const TransparentBuffer& other) = delete;

  // returns true if succeeded
  bool initialize();

  // bind and unbind
  void bindForInput();
  void unbindFromInput();
  void bindForOutput();
  void bindForOutput(unsigned int unit);

private:

  // the handle for the buffer
  GLuint m_buffer_handle;
  GLuint m_depth_handle;

  // the texture
  Texture *  m_buffer_texture;
  Texture * m_position_texture;

};

#endif // RENDERBUFFER_H