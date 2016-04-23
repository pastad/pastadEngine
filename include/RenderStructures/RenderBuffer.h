#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include <string>

#include "gl_core_4_.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderBuffer
{
public:
    RenderBuffer();
    ~RenderBuffer();

    RenderBuffer(const RenderBuffer& other) = delete;
    RenderBuffer& operator=(const RenderBuffer& other) = delete;

    // returns true if succeeded
    bool initialize();

    // bind and unbind
    void bindForInput();
    void unbindFromInput();
    void bindForOutput();

    // creates the texture
    void createTexture( GLenum texUnit, GLenum format, GLuint &texid) ;

private:

    // the handle for the buffer
    GLuint m_buffer_handle;

    // the texture handles
    GLuint m_buffer_texture;

};

#endif // RENDERBUFFER_H