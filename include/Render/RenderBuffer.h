#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include <string>

#define GLM_FORCE_RADIANS
#include "gl_core_4_.hpp"
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
  bool initializeWithDepth();

  // bind and unbind
  void bindForInput();
  void unbindFromInput();
  void bindForOutput();
  void bindForOutput(unsigned int unit);

  // computes the luminance for HDR
  float getAverageLuminance();

private:

  // the handle for the buffer
  GLuint m_buffer_handle;
  GLuint m_depth_handle;

  // the texture
  Texture *  m_buffer_texture;

};

#endif // RENDERBUFFER_H