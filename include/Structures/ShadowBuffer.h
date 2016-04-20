#ifndef SHADOWBUFFER_H
#define SHADOWBUFFER_H

#include <string>

#include "gl_core_4_.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShadowBuffer
{
public:
    ShadowBuffer();
    ~ShadowBuffer();

    ShadowBuffer(const ShadowBuffer& other) = delete;
    ShadowBuffer& operator=(const ShadowBuffer& other) = delete;

    // returns true if succeeded
    bool initialize(  float width ,  float height);

    // bind and unbind
    void bindForInput();
    void unbindFromInput();
    void bindForOutput(unsigned int offset);


private:

    // the handle for the buffer
    GLuint m_buffer_handle;

    // the texture handles
    GLuint m_depth_texture;

};

#endif // SHADOWBUFFER_H