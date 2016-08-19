#include "Texture.h"
#include "SOIL.h"
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
  gl::DeleteTextures(1, &m_texture);
}


//  load/create -------------------------------------------------

bool Texture::load()
{
  if(m_path == "")
    return false;
  gl::GenTextures(1, &m_texture);
  gl::BindTexture(gl::TEXTURE_2D, m_texture);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::REPEAT);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::REPEAT);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);

  unsigned char* image = SOIL_load_image(m_path.c_str(), &m_width, &m_height, 0, SOIL_LOAD_RGBA);
  if(image == nullptr )
  {
      Engine::getLog()->log("Texture", "loading failed");
      printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
      return false;
  }

  gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA, m_width, m_height, 0, gl::RGBA, gl::UNSIGNED_BYTE, image);
  gl::GenerateMipmap(gl::TEXTURE_2D);
  SOIL_free_image_data(image);
  gl::GenerateMipmap(gl::TEXTURE_2D);
  gl::BindTexture(gl::TEXTURE_2D, 0);
  m_loaded = true;

  return true;
}

void Texture::create(int unit , GLenum format, int width, int height)
{
  m_width = width;
  m_height = height;
  gl::ActiveTexture(gl::TEXTURE0 +unit);
  gl::GenTextures(1, &m_texture);
  gl::BindTexture(gl::TEXTURE_2D, m_texture);
  gl::TexStorage2D(gl::TEXTURE_2D, 1, format, width,  height);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST);
}


//  bind/clear -------------------------------------------------

void Texture::bind(unsigned int unit)
{
  gl::ActiveTexture(gl::TEXTURE0 + unit);
  gl::BindTexture(gl::TEXTURE_2D, m_texture);
}

void Texture::bindToFramebuffer(GLenum attachement)
{
  gl::FramebufferTexture2D(gl::FRAMEBUFFER, attachement, gl::TEXTURE_2D, m_texture, 0);
}

void Texture::clear(int unit, unsigned int depth)
{
  std::vector<GLubyte> emptyData(m_width * m_height * depth, 0);
  gl::ActiveTexture(gl::TEXTURE0 +unit);
  gl::BindTexture(gl::TEXTURE_2D, m_texture);
  gl::TexSubImage2D(gl::TEXTURE_2D, 0, 0, 0, m_width, m_height, gl::RGB, gl::UNSIGNED_BYTE, &emptyData[0]);
}


//  getter/setters -------------------------------------------------

glm::vec2 Texture::getSize()
{
  return glm::vec2(m_width,m_height);
}