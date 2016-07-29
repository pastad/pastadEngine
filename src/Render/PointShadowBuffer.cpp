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

bool PointShadowBuffer::initialize(  float width ,  float height)
{
  /*for(int x=0; x< 6;x++)
  {
    m_buffers[x] = new DirectionalShadowBuffer();
    if( !m_buffers[x]->initialize(width,height))
      return false;
  }*/
  m_width = width;
  m_height = height;
  glGenFramebuffers(1, &m_buffer_handle);

   GLfloat border[] = {1000.0f, 0.0f,0.0f,0.0f };

  glGenTextures(1, &m_depth_texture);
  glBindTexture(GL_TEXTURE_2D, m_depth_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenTextures(1, &m_cube_texture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_cube_texture);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

  for (GLuint i = 0; i < 6; ++i)
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, m_width, m_height, 0, GL_RED, GL_FLOAT, NULL);  


  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer_handle);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, m_depth_texture, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    Engine::getLog()->log("PointShadowBuffer", "FBO couldn't be created");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}

//  bind/unbind -------------------------------------------------

void PointShadowBuffer::bindForInput(GLenum face)
{
  //m_buffers[iteration]->bindForInput(); 
  glViewport(0, 0, Engine::getWindowWidth(), Engine::getWindowHeight());
  //glBindFramebuffer(GL_FRAMEBUFFER, m_buffer_handle);


  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_buffer_handle);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, face, m_cube_texture, 0);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);

  glClear(GL_DEPTH_BUFFER_BIT);
}

void PointShadowBuffer::unbindFromInput()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PointShadowBuffer::bindForOutput(unsigned int offsetstart)
{
  glActiveTexture(GL_TEXTURE0+ TEXTURE_POINT_SHADOW_START+offsetstart);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_cube_texture);
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