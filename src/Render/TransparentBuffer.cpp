#include "TransparentBuffer.h"


#include "Engine.h"
#include "Texture.h"

#include <vector>

TransparentBuffer::TransparentBuffer()
{
}

TransparentBuffer::~TransparentBuffer()
{
  delete m_buffer_texture;
  delete m_position_texture;
  gl::DeleteFramebuffers(1, &m_buffer_handle);
  gl::DeleteRenderbuffers(1, &m_depth_handle);
}


//  initialization -------------------------------------------------

bool TransparentBuffer::initialize()
{
  gl::GenFramebuffers(1, &m_buffer_handle);
  gl::BindFramebuffer(gl::FRAMEBUFFER, m_buffer_handle);

  gl::GenRenderbuffers(1, &m_depth_handle);
  gl::BindRenderbuffer(gl::RENDERBUFFER, m_depth_handle);
  gl::RenderbufferStorage(gl::RENDERBUFFER, gl::DEPTH_COMPONENT, Engine::getWindowWidth(), Engine::getWindowHeight());

  gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, m_depth_handle);

  m_buffer_texture = new Texture();
  m_buffer_texture->create(0, gl::RGBA8, Engine::getWindowWidth(), Engine::getWindowHeight());

  m_buffer_texture->bindToFramebuffer(gl::COLOR_ATTACHMENT2);

  m_position_texture = new Texture();
  m_position_texture->create(0, gl::RGB32F, Engine::getWindowWidth(), Engine::getWindowHeight());

  m_position_texture->bindToFramebuffer(gl::COLOR_ATTACHMENT0);

  GLenum drawBuffers[] = {gl::NONE, gl::COLOR_ATTACHMENT0, gl::COLOR_ATTACHMENT2 };
  gl::DrawBuffers(3, drawBuffers);

  gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

  return true;
}

//  bind/unbind -------------------------------------------------

void TransparentBuffer::bindForInput()
{
  gl::BindFramebuffer(gl::FRAMEBUFFER, m_buffer_handle);
}

void TransparentBuffer::unbindFromInput()
{
  gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}

void TransparentBuffer::bindForOutput()
{
  bindForOutput(0);
}

void TransparentBuffer::bindForOutput(unsigned int unit)
{
  m_buffer_texture->bind(unit);
  m_position_texture->bind(unit+1);
}