#include "gl_core_4_.hpp"

#define CLOTH_PARTICLE_SIZE_X 40
#define CLOTH_PARTICLE_SIZE_Y 40

#define PRIM_RESTART 0xffffff

class Transform;
class RenderShader;
class ComputeShader;

class Cloth
{
public:
  Cloth();
  ~Cloth();

  bool init(float x, float y);

  void compute(ComputeShader * cs);

  void render(RenderShader * rs);

private:
  Transform * m_transform;


  GLuint m_vao;
  GLuint m_num_elements;

  GLuint m_read_buffer;
  GLuint m_pos_buffer[2], m_vel_buffer[2];
  GLuint m_normal_buffer, m_el_buffer, m_tex_coord_buffer;
};

