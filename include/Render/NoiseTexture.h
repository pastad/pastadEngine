#ifndef NOISE_TEXTURE_H
#define NOISE_TEXTURE_H

#pragma once


#include <string>
#include <glm/glm.hpp>
#include "Engine.h"


class NoiseTexture
{
private:
  int m_width;
  int m_height;

  // gl handle
  GLuint m_texture;

  // indicates loading state
  bool m_loaded;

public:
  NoiseTexture();
  ~NoiseTexture();

  // loads a texture from a file
  bool create();

  // creates a new texture , for example to write in
  void create(int unit, GLenum format, int width, int height);

  void bind(unsigned int unit);

  // binds texture to the framebuffer
  void bindToFramebuffer(GLenum attachement);
};

#endif