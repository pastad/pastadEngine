#ifndef POINTSHADOWBUFFER_H
#define POINTSHADOWBUFFER_H

#include <string>

#include "gl_core_4_.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class PointShadowBuffer
{
public:
    PointShadowBuffer();
    ~PointShadowBuffer();

    PointShadowBuffer(const PointShadowBuffer& other) = delete;
    PointShadowBuffer& operator=(const PointShadowBuffer& other) = delete;

    // returns true if succeeded
    bool initialize(  float width ,  float height);

    // bind and unbind
    void bindForInput(GLenum face);
    void unbindFromInput();
    void bindForOutput(unsigned int offsetstart);

    // getters for width and height
    int getWidth();
    int getHeight();


private:

    // the buffer handle
    GLuint m_buffer_handle;

    // the texture handle
    GLuint m_depth_texture;
    GLuint m_cube_texture;

    // the width and the height of the buffer;
    unsigned int m_width;
    unsigned int m_height;

   
};

#endif // POINTSHADOWBUFFER_H