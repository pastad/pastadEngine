#include "GBuffer.h"

#include "Engine.h"

#include <vector>

GBuffer::GBuffer()
{
}
GBuffer::~GBuffer()
{  
}

bool GBuffer::initialize()
{
 
  glGenFramebuffers(1, &m_buffer_handle);
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer_handle);

  
  glGenRenderbuffers(1, &m_depth_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Engine::getWindowWidth(), Engine::getWindowHeight());

  createTexture(GL_TEXTURE0, GL_RGB32F, m_position_texture);  // Position
  createTexture(GL_TEXTURE1, GL_RGB32F, m_normal_texture); // Normal
  createTexture(GL_TEXTURE2, GL_RGB8, m_color_texture);  // Color
  createTexture(GL_TEXTURE3, GL_R3_G3_B2, m_material_texture);  // Material

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_buffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_position_texture, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normal_texture, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_color_texture, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_material_texture, 0);

  GLenum drawBuffers[] = {GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                      GL_COLOR_ATTACHMENT2,  GL_COLOR_ATTACHMENT3};
  glDrawBuffers(5, drawBuffers);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}

void GBuffer::createTexture( GLenum texUnit, GLenum format, GLuint &texid ) 
{
  glActiveTexture(texUnit);
  glGenTextures(1, &texid);
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexStorage2D(GL_TEXTURE_2D, 1, format, Engine::getWindowWidth(),  Engine::getWindowHeight());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


void GBuffer::bindForInput()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer_handle);

  std::vector<GLubyte> emptyData(Engine::getWindowWidth() * Engine::getWindowHeight() * 4, 0);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, m_color_texture); 
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Engine::getWindowWidth(), Engine::getWindowHeight(), GL_RGB, GL_UNSIGNED_BYTE, &emptyData[0]);

}
void GBuffer::unbindFromInput()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void GBuffer::bindForOutput()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_position_texture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_normal_texture);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, m_color_texture); 
 
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, m_material_texture);
}