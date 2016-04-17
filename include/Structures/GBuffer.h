#ifndef GBUFFER_H
#define GBUFFER_H

#include <string>

#include "gl_core_4_.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    // creates a texture for the gbuffer
    void createTexture( GLenum texUnit, GLenum format, GLuint &texid ) ;

private:

    // the handle for the buffer
    GLuint m_buffer_handle;

    // the texture handles
    GLuint m_depth_buffer;
    GLuint m_position_texture; 
    GLuint m_normal_texture;
    GLuint m_color_texture;
    GLuint m_material_texture;


};

#endif // GBUFFER_H