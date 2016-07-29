#ifndef GBUFFER_H
#define GBUFFER_H

#include <string>

#define GLM_FORCE_RADIANS
#include "gl_core_4_.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture;

class GBuffer
{
public:
    GBuffer();
    ~GBuffer();

    GBuffer(const GBuffer& other) = delete;
    GBuffer& operator=(const GBuffer& other) = delete;

    // returns true if succeeded
    bool initialize();

    // bind and unbind
    void bindForInput();
    void unbindFromInput();
    void bindForOutput();

    int pickObjectAt(glm::vec2 pos);
    int pickObjectAtCenter();

private:

    // the handle for the buffer
    GLuint m_buffer_handle;

    // the texture handles
    GLuint m_depth_buffer;
    Texture * m_position_texture; 
    Texture * m_normal_texture;
    Texture * m_color_texture;
    Texture * m_material_texture;
};

#endif // GBUFFER_H