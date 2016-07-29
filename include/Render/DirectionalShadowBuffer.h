#ifndef DIRECTIONALSHADOWBUFFER_H
#define DIRECTIONALSHADOWBUFFER_H

#include <string>

#define GLM_FORCE_RADIANS
#include "gl_core_4_.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DirectionalShadowBuffer
{
public:
    DirectionalShadowBuffer();
    ~DirectionalShadowBuffer();

    DirectionalShadowBuffer(const DirectionalShadowBuffer& other) = delete;
    DirectionalShadowBuffer& operator=(const DirectionalShadowBuffer& other) = delete;

    // returns true if succeeded
    bool initialize(  float width ,  float height);

    // bind and unbind
    void bindForInput();
    void unbindFromInput();
    void bindForOutput(unsigned int offset);

    // getters for width and height
    int getWidth();
    int getHeight();


private:

    // the handle for the buffer
    GLuint m_buffer_handle;

    // the texture handles
    GLuint m_depth_texture;

    // the width and height of the  buffer
    int m_width;
    int m_height;

};

#endif // DIRECTIONALSHADOWBUFFER_H