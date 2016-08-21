#include "PointShadowBuffer.h"

#include "Engine.h"
#include "Log.h"
#include "Texture.h"
#include "DirectionalShadowBuffer.h"

PointShadowBuffer::PointShadowBuffer()
{  
}

PointShadowBuffer::~PointShadowBuffer()
{  
  //for(int x=0; x< 6;x++)
  //  delete m_buffers[x];
}


//  initialization -------------------------------------------------

bool PointShadowBuffer::initialize(  int width ,  int height)
{
  /*for(int x=0; x< 6;x++)
  {
    m_buffers[x] = new DirectionalShadowBuffer();
    if( !m_buffers[x]->initialize(width,height))
      return false;
  }*/
  m_width = width;
  m_height = height;
  gl::GenFramebuffers(1, &m_buffer_handle);

   GLfloat border[] = {1000.0f, 0.0f,0.0f,0.0f };

  gl::GenTextures(1, &m_depth_texture);
  gl::BindTexture(gl::TEXTURE_2D, m_depth_texture);
  gl::TexImage2D(gl::TEXTURE_2D, 0, gl::DEPTH_COMPONENT32, width, height, 0, gl::DEPTH_COMPONENT, gl::FLOAT, NULL);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);

  gl::TexParameterfv(gl::TEXTURE_2D, gl::TEXTURE_BORDER_COLOR, border);
  gl::BindTexture(gl::TEXTURE_2D, 0);

  gl::GenTextures(1, &m_cube_texture);
  gl::BindTexture(gl::TEXTURE_CUBE_MAP, m_cube_texture);
  gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
  gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
  gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
  gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
  gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_R, gl::CLAMP_TO_EDGE);
  gl::TexParameterfv(gl::TEXTURE_2D, gl::TEXTURE_BORDER_COLOR, border);

  for (GLuint i = 0; i < 6; ++i)
    gl::TexImage2D(gl::TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, gl::R32F, m_width, m_height, 0, gl::RED, gl::FLOAT, NULL);


  gl::BindFramebuffer(gl::FRAMEBUFFER, m_buffer_handle);
  gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT,gl::TEXTURE_2D, m_depth_texture, 0);
  gl::DrawBuffer(gl::NONE);
  gl::ReadBuffer(gl::NONE);
  if (gl::CheckFramebufferStatus(gl::FRAMEBUFFER) != gl::FRAMEBUFFER_COMPLETE)
    Engine::getLog()->log("PointShadowBuffer", "FBO couldn't be created");
  gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

  return true;
}

//  bind/unbind -------------------------------------------------

void PointShadowBuffer::bindForInput(GLenum face)
{
  //m_buffers[iteration]->bindForInput(); 
  gl::Viewport(0, 0, Engine::getWindowWidth(), Engine::getWindowHeight());
  //glBindFramebuffer(gl::FRAMEBUFFER, m_buffer_handle);


  gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, m_buffer_handle);
  gl::FramebufferTexture2D(gl::DRAW_FRAMEBUFFER, gl::COLOR_ATTACHMENT0, face, m_cube_texture, 0);
  gl::DrawBuffer(gl::COLOR_ATTACHMENT0);

  gl::Clear(gl::DEPTH_BUFFER_BIT);
}

void PointShadowBuffer::unbindFromInput()
{
  gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}

void PointShadowBuffer::bindForOutput(unsigned int offsetstart)
{
  gl::ActiveTexture(gl::TEXTURE0+ TEXTURE_POINT_SHADOW_START+offsetstart);
  gl::BindTexture(gl::TEXTURE_CUBE_MAP, m_cube_texture);
}


//  getter/setters -------------------------------------------------

int PointShadowBuffer::getWidth()
{
  return m_width;
}
int PointShadowBuffer::getHeight()
{
  return m_height;
} 