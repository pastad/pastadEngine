#include "NoiseTexture.h"



#include <iostream>
#include <sstream>
#include <random>

NoiseTexture::NoiseTexture():m_loaded(false)
{
}


NoiseTexture::~NoiseTexture()
{
  gl::DeleteTextures(1, &m_texture);
}

bool NoiseTexture::create()
{
  std::uniform_real_distribution<GLfloat> rf(0.0, 1.0);
  std::default_random_engine dre;

  gl::GenTextures(1, &m_texture);

  std::vector<glm::vec3> noise;

  for(int i = 0; i < 36;i++)
  {
    glm::vec3 n(rf(dre) * 2.0 - 1.0, rf(dre) * 2.0 - 1.0, 0.0f);
    noise.push_back(n);
  }
  gl::GenTextures(1, &m_texture);

  gl::BindTexture(gl::TEXTURE_2D, m_texture);

  gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGB16F, 6, 6, 0, gl::RGB, gl::FLOAT, &noise[0]);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::REPEAT);
  gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::REPEAT);

  return true;
}


void NoiseTexture::bindToFramebuffer(GLenum attachement)
{
  gl::FramebufferTexture2D(gl::FRAMEBUFFER, attachement, gl::TEXTURE_2D, m_texture, 0);
}

void NoiseTexture::bind(unsigned int unit)
{
  gl::ActiveTexture(gl::TEXTURE0 + unit);
  gl::BindTexture(gl::TEXTURE_2D, m_texture);
}