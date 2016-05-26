#include "JitterTexture.h"
#define M_PI 3.14159265358979323846
// inspired by https://github.com/daw42/glslcookbook

JitterTexture::JitterTexture()
{  
}

JitterTexture::~JitterTexture()
{  
}

bool JitterTexture::load()
{
  unsigned int gen_size_x = 4;
  unsigned int gen_size_y = 8;

  unsigned int size = 8;
  unsigned int gen_size = gen_size_x * gen_size_y;

  unsigned int buffer_size = size * size * gen_size * 2;

  float * texture_gen = new float[buffer_size];

  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &m_texture_handle);
  glBindTexture(GL_TEXTURE_3D, m_texture_handle);
  glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA32F, size, size, gen_size/2);

  for( unsigned int o = 0; o < size; o++) 
  {
    for(unsigned int p = 0; p < size; p++) 
    {
      for(unsigned int gen_count = 0; gen_count < gen_size; gen_count+= 2) 
      {   
        glm::vec4 gen_jitter;  

        int x =    gen_count % gen_size_x;
        int x_2 = (gen_count+1) % gen_size_x;
        int y = (gen_size - 1 - gen_count) / gen_size_x;        
        int y_2 = (gen_size - 1 -gen_count - 1) / gen_size_x;
        
        gen_jitter.x = x + ((float)rand() / RAND_MAX) ;
        gen_jitter.x /= gen_size_x;

        gen_jitter.y = y + ((float)rand() / RAND_MAX)  ;
        gen_jitter.y /= gen_size_y;

        gen_jitter.z = x_2 + ((float)rand() / RAND_MAX)  ;
        gen_jitter.z /= gen_size_x;

        gen_jitter.w = y_2  + ((float)rand() / RAND_MAX) ;
        gen_jitter.w /= gen_size_y;      
        
        unsigned int ins_pos = (gen_count/2)* size*size + o * size + p ;
        ins_pos *= 4;

        texture_gen[ins_pos + 0] =  cosf( M_PI *2.0f * gen_jitter.x ) * sqrtf(gen_jitter.y) ;
        texture_gen[ins_pos + 1] = sinf(M_PI *2.0f * gen_jitter.x) *sqrtf(gen_jitter.y);
        texture_gen[ins_pos + 2] =  cosf(M_PI *2.0f * gen_jitter.z ) * sqrtf(gen_jitter.w) ;
        texture_gen[ins_pos + 3] = sinf(M_PI *2.0f * gen_jitter.z) *sqrtf(gen_jitter.w);
      }
    }
  }

  glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, size, size, gen_size/2, GL_RGBA, GL_FLOAT, texture_gen);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  delete [] texture_gen;
  return true;
}
void JitterTexture::bind(int offset)
{
  glActiveTexture(GL_TEXTURE0 + offset);
  glBindTexture(GL_TEXTURE_3D, m_texture_handle);
}