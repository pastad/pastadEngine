#ifndef JITTERTEXTURE_H
#define JITTERTEXTURE_H

#include "gl_core_4_.hpp"
#include <glm/glm.hpp>

// jitter texture for shadow smoothing
// inpired by OpenGL 4 Shading Language Cookbook

class JitterTexture 
{
public:
  JitterTexture();
  virtual ~JitterTexture();
  JitterTexture(const JitterTexture& other) = delete;
  JitterTexture& operator=(const JitterTexture& other) = delete;

  // loads the texture | initializes it randomly
  bool load();

  // binds the jitter texture
  void bind(int offset);
  

protected:

  GLuint m_texture_handle;


};

#endif // JITTERTEXTURE_H