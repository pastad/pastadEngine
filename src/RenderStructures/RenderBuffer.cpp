#include "RenderBuffer.h"

#include "Engine.h"
#include "Texture.h"

#include <vector>

RenderBuffer::RenderBuffer()
{
}
RenderBuffer::~RenderBuffer()
{  
  delete m_buffer_texture;
  glDeleteFramebuffers(1, &m_buffer_handle);  
}

bool RenderBuffer::initialize()
{
 
  glGenFramebuffers(1, &m_buffer_handle);
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer_handle);  

  m_buffer_texture = new Texture();
  m_buffer_texture->create(0, GL_RGB32F,Engine::getWindowWidth(),  Engine::getWindowHeight());
  m_buffer_texture->bindToFramebuffer(GL_COLOR_ATTACHMENT0);
  
  GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, drawBuffers);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
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
  m_buffer_texture->bind(0);
}