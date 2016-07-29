#include "GBuffer.h"

#include "Engine.h"
#include "Texture.h"

#include <vector>
#include <iostream>

GBuffer::GBuffer()
{
}

GBuffer::~GBuffer()
{  
  glDeleteFramebuffers(1, &m_buffer_handle);
  glDeleteTextures(1, &m_depth_buffer);

  delete m_position_texture;
  delete m_normal_texture;
  delete m_color_texture;
  delete m_material_texture;  
}

//  initialization -------------------------------------------------

bool GBuffer::initialize()
{
 
  glGenFramebuffers(1, &m_buffer_handle);
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer_handle);
  
  glGenRenderbuffers(1, &m_depth_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depth_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Engine::getWindowWidth(), Engine::getWindowHeight());
 
  m_position_texture = new Texture();
  m_position_texture->create(0, GL_RGB32F,Engine::getWindowWidth(),  Engine::getWindowHeight());
  
  m_normal_texture = new Texture();
  m_normal_texture->create(0, GL_RGB32F,Engine::getWindowWidth(),  Engine::getWindowHeight());
  
  m_color_texture =  new Texture();
  m_color_texture->create(0, GL_RGB8,Engine::getWindowWidth(),  Engine::getWindowHeight());
  
  m_material_texture =  new Texture();
  m_material_texture->create(0, GL_RGB32F,Engine::getWindowWidth(),  Engine::getWindowHeight());
  
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_buffer);
 
  m_position_texture->bindToFramebuffer(GL_COLOR_ATTACHMENT0);
  m_normal_texture->bindToFramebuffer(GL_COLOR_ATTACHMENT1);  
  m_color_texture->bindToFramebuffer(GL_COLOR_ATTACHMENT2);  
  m_material_texture->bindToFramebuffer(GL_COLOR_ATTACHMENT3);  

  GLenum drawBuffers[] = {GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                      GL_COLOR_ATTACHMENT2,  GL_COLOR_ATTACHMENT3};
  glDrawBuffers(5, drawBuffers);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}


//  bind/unbind -------------------------------------------------

void GBuffer::bindForInput()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer_handle);
  m_color_texture->clear(2,4);
}

void GBuffer::unbindFromInput()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::bindForOutput()
{
  m_position_texture->bind(0);
  m_normal_texture->bind(1);
  m_color_texture->bind(2);
  m_material_texture->bind(3);
}


//  picking -------------------------------------------------

int GBuffer::pickObjectAtCenter()
{
  glm::vec2 pos( Engine::getWindowWidth() / 2,Engine::getWindowHeight()/2);
  return pickObjectAt(pos);
}

int GBuffer::pickObjectAt(glm::vec2 pos)
{
  int size = Engine::getWindowWidth() *  Engine::getWindowHeight();
  GLfloat *texData = new GLfloat[ size*3];

  int p  =(int)pos.y * Engine::getWindowWidth() *3 ;
  p+= (int)pos.x * 3;
  p+=1;

  m_material_texture->bind(0);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, texData);
  int ret = texData[p];
  //std::cout << "chosen: "<<ret <<" left: "<< texData[p-1] <<" right: "<< texData[p+1]<<std::endl;

  delete [] texData;
  return ret;
}

