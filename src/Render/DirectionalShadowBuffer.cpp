#include "DirectionalShadowBuffer.h"


#include "Log.h"
#include "Texture.h"

#include <sstream>

DirectionalShadowBuffer::DirectionalShadowBuffer()
{  
}

DirectionalShadowBuffer::~DirectionalShadowBuffer()
{  
  gl::DeleteFramebuffers(1, &m_buffer_handle);
  gl::DeleteTextures(1, &m_depth_texture);
}


//  initialization -------------------------------------------------

bool DirectionalShadowBuffer::initialize(  int width ,  int height)
{
  GLfloat border[] = {1.0f, 0.0f,0.0f,0.0f };
  m_width = width;
  m_height = height;
  
  gl::GenTextures(1, &m_depth_texture);
  gl::BindTexture(gl::TEXTURE_2D, m_depth_texture);
  gl::TexStorage2D(gl::TEXTURE_2D, 1, gl::DEPTH_COMPONENT24, width, height);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_BORDER);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_BORDER);
  gl::TexParameterfv(gl::TEXTURE_2D, gl::TEXTURE_BORDER_COLOR, border);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_COMPARE_MODE, gl::COMPARE_REF_TO_TEXTURE);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_COMPARE_FUNC, gl::LESS);

  gl::ActiveTexture(gl::TEXTURE0);
  gl::BindTexture(gl::TEXTURE_2D, m_depth_texture);

  gl::GenFramebuffers(1, &m_buffer_handle);
  gl::BindFramebuffer(gl::FRAMEBUFFER, m_buffer_handle);
  gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::TEXTURE_2D, m_depth_texture, 0);

  GLenum drawBuffers[] = { gl::NONE};
  gl::DrawBuffers(1, drawBuffers);

  GLenum result = gl::CheckFramebufferStatus(gl::FRAMEBUFFER);
  if( result != gl::FRAMEBUFFER_COMPLETE)
  {
    Engine::getLog()->log("DirectionalShadowBuffer", "couldn't be inititalized" );
    return false;
  }

  gl::BindFramebuffer(gl::FRAMEBUFFER,0);

  return true;
}


//  bind/unbind -------------------------------------------------

void DirectionalShadowBuffer::bindForInput()
{
  gl::BindFramebuffer(gl::FRAMEBUFFER, m_buffer_handle);
}

void DirectionalShadowBuffer::unbindFromInput()
{
  gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}

void DirectionalShadowBuffer::bindForOutput(unsigned int offset)
{
  gl::ActiveTexture(gl::TEXTURE0+ TEXTURE_SHADOW_START + offset);
  gl::BindTexture(gl::TEXTURE_2D, m_depth_texture);
  std::stringstream ss;
//  ss <<(TEXTURE_SHADOW_START + offset);
 // Engine::getLog()->log("DirectionalShadowBuffer", " bound buffer to", ss.str());
}


//  getter/setter -------------------------------------------------

int DirectionalShadowBuffer::getWidth()
{
  return m_width;
}

int DirectionalShadowBuffer::getHeight()
{
  return m_height;
} 