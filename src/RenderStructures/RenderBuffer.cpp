#include "RenderBuffer.h"

#include "Engine.h"

#include <vector>

RenderBuffer::RenderBuffer()
{
}
RenderBuffer::~RenderBuffer()
{  
}

bool RenderBuffer::initialize()
{
 
  glGenFramebuffers(1, &m_buffer_handle);
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer_handle);  

  createTexture(GL_TEXTURE0, GL_RGB32F, m_buffer_texture);  
  
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_buffer_texture, 0);
  
  GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, drawBuffers);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}

void RenderBuffer::createTexture( GLenum texUnit, GLenum format, GLuint &texid) 
{
  glActiveTexture(texUnit);
  glGenTextures(1, &texid);
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexStorage2D(GL_TEXTURE_2D, 1, format, Engine::getWindowWidth(),  Engine::getWindowHeight());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void RenderBuffer::bindForInput()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer_handle);
}
void RenderBuffer::unbindFromInput()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void RenderBuffer::bindForOutput()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_buffer_texture);
}