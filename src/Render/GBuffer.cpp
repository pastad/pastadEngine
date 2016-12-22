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
  gl::DeleteFramebuffers(1, &m_buffer_handle);
  gl::DeleteTextures(1, &m_depth_buffer);

  delete m_position_texture;
  delete m_normal_texture;
  delete m_color_texture;
  delete m_material_texture;  
}

//  initialization -------------------------------------------------

bool GBuffer::initialize()
{
 
  gl::GenFramebuffers(1, &m_buffer_handle);
  gl::BindFramebuffer(gl::FRAMEBUFFER, m_buffer_handle);
  
  gl::GenRenderbuffers(1, &m_depth_buffer);
  gl::BindRenderbuffer(gl::RENDERBUFFER, m_depth_buffer);
  gl::RenderbufferStorage(gl::RENDERBUFFER, gl::DEPTH_COMPONENT, Engine::getWindowWidth(), Engine::getWindowHeight());
 
  m_position_texture = new Texture();
  m_position_texture->create(0, gl::RGB32F,Engine::getWindowWidth(),  Engine::getWindowHeight());
  
  m_normal_texture = new Texture();
  m_normal_texture->create(0, gl::RGB32F,Engine::getWindowWidth(),  Engine::getWindowHeight());
  
  m_color_texture =  new Texture();
  m_color_texture->create(0, gl::RGBA8,Engine::getWindowWidth(),  Engine::getWindowHeight());
  
  m_material_texture =  new Texture();
  m_material_texture->create(0, gl::RGB32F,Engine::getWindowWidth(),  Engine::getWindowHeight());
  
  gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, m_depth_buffer);
 
  m_position_texture->bindToFramebuffer(gl::COLOR_ATTACHMENT0);
  m_normal_texture->bindToFramebuffer(gl::COLOR_ATTACHMENT1);  
  m_color_texture->bindToFramebuffer(gl::COLOR_ATTACHMENT2);  
  m_material_texture->bindToFramebuffer(gl::COLOR_ATTACHMENT3);  

  GLenum drawBuffers[] = {gl::NONE, gl::COLOR_ATTACHMENT0, gl::COLOR_ATTACHMENT1,
                      gl::COLOR_ATTACHMENT2,  gl::COLOR_ATTACHMENT3};
  gl::DrawBuffers(5, drawBuffers);

  gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

  return true;
}


//  bind/unbind -------------------------------------------------

void GBuffer::bindForInput()
{
  gl::BindFramebuffer(gl::FRAMEBUFFER, m_buffer_handle);
//  m_color_texture->clear(2,4);
}

void GBuffer::unbindFromInput()
{
  gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}

void GBuffer::bindForOutput()
{
  bindForOutput(0);
}

void GBuffer::bindForOutput(int offset)
{
  m_position_texture->bind(0 + offset);
  m_normal_texture->bind(1 + offset);
  m_color_texture->bind(2 + offset);
  m_material_texture->bind(3 + offset);
}

void GBuffer::bindMaterialTextureForOutput(unsigned int off)
{
  m_material_texture->bind(off);
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
  gl::GetTexImage(gl::TEXTURE_2D, 0, gl::RGB, gl::FLOAT, texData);
  if( p >= size *3)
    return 0;

  int ret = texData[p];
  //std::cout << "chosen: "<<ret <<" left: "<< texData[p-1] <<" right: "<< texData[p+1]<<std::endl;

  delete [] texData;
  return ret;
}

