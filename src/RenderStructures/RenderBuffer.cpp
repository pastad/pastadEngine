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

float RenderBuffer::getAverageLuminance()
{
  int size = Engine::getWindowWidth()*Engine::getWindowHeight();
  GLfloat *texData = new GLfloat[ size*3];

  m_buffer_texture->bind(0);
  //glActiveTexture(GL_TEXTURE0);
  //glBindTexture(GL_TEXTURE_2D, hdrTex);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, texData);

  float sampling = 1.0f;

  float sum = 0.0f;
  for( int i = 0; i < size; i+=sampling )  // 100 just to make it faster
  {
   float lum = glm::dot(glm::vec3(texData[i*3+0], texData[i*3+1], texData[i*3+2]),
       glm::vec3(0.2126f, 0.7152f, 0.0722f) );
   sum += logf( lum + 0.00001f );
  }

  //prog.setUniform( "AveLum", expf( sum / (Engine::getWindowWidth(*Engine::getWindowHeight()) ) );
  //printf("(%f)\n", exp( sum / (width*height) ) );
  delete [] texData;

  return expf( sum / (size/sampling) );// 100 again to make it faster
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
  bindForOutput(0);
}
void RenderBuffer::bindForOutput(unsigned int unit)
{
  m_buffer_texture->bind(unit);
}