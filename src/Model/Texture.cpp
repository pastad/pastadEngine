#include "Texture.h"
#include <SOIL/SOIL.h>
#include "RenderSubsystem.h"
#include "RenderShader.h"

#include "Log.h"

Texture::Texture(std::string path):m_path(path),m_loaded(false)
{
}

Texture::Texture():m_loaded(false)
{
}

Texture::~Texture()
{
  glDeleteTextures(1, &m_texture);
}


//  load/create -------------------------------------------------

bool Texture::load()
{
  if(m_path == "")
    return false;
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned char* image = SOIL_load_image(m_path.c_str(), &m_width, &m_height, 0, SOIL_LOAD_RGBA);
  if(image == nullptr )
  {
      Engine::getLog()->log("Texture", "loading failed");
      printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
      return false;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  m_loaded = true;

  return true;
}

void Texture::create(int unit , GLenum format, int width, int height)
{
  m_width = width;
  m_height = height;
  glActiveTexture(GL_TEXTURE0 +unit);
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexStorage2D(GL_TEXTURE_2D, 1, format, width,  height);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


//  bind/clear -------------------------------------------------

void Texture::bind(unsigned int unit)
{
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::bindToFramebuffer(GLenum attachement)
{
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachement, GL_TEXTURE_2D, m_texture, 0);
}

void Texture::clear(int unit, unsigned int depth)
{
  std::vector<GLubyte> emptyData(m_width * m_height * depth, 0);
  glActiveTexture(GL_TEXTURE0 +unit);
  glBindTexture(GL_TEXTURE_2D, m_texture); 
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, &emptyData[0]);
}


//  getter/setters -------------------------------------------------

glm::vec2 Texture::getSize()
{
  return glm::vec2(m_width,m_height);
}