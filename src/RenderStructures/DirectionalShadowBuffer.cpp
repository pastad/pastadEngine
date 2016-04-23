#include "DirectionalShadowBuffer.h"

#include "Engine.h"
#include "Log.h"

DirectionalShadowBuffer::DirectionalShadowBuffer()
{  
}
DirectionalShadowBuffer::~DirectionalShadowBuffer()
{  
}

bool DirectionalShadowBuffer::initialize(  float width ,  float height)
{
  GLfloat border[] = {1.0f, 0.0f,0.0f,0.0f };
  m_width = width;
  m_height =height;
  
  glGenTextures(1, &m_depth_texture);
  glBindTexture(GL_TEXTURE_2D, m_depth_texture);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width, height);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_depth_texture);

  glGenFramebuffers(1, &m_buffer_handle);
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer_handle);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);

  GLenum drawBuffers[] = {GL_NONE};
  glDrawBuffers(1, drawBuffers);

  GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if( result != GL_FRAMEBUFFER_COMPLETE)
  {
    Engine::getLog()->log("DirectionalShadowBuffer", "couldn't be inititalized" );
    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER,0);

  return true;
}

void DirectionalShadowBuffer::bindForInput()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer_handle);
}
void DirectionalShadowBuffer::unbindFromInput()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void DirectionalShadowBuffer::bindForOutput(unsigned int offset)
{
  glActiveTexture(GL_TEXTURE0+ 20 + offset);
  glBindTexture(GL_TEXTURE_2D, m_depth_texture);
}

int DirectionalShadowBuffer::getWidth()
{
  return m_width;
}
int DirectionalShadowBuffer::getHeight()
{
  return m_height;
} 