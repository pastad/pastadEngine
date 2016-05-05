#ifndef SKYBOX_H
#define SKYBOX_H

#include "Engine.h"
#include <string>
#include <glm/glm.hpp>

// class to represent the skybox

class Skybox
{
  public:
    Skybox();
    ~Skybox();
    Skybox(const Skybox& other) = delete;
    Skybox& operator=(const Skybox& other) = delete;

    // loads the skybox
    bool load(const std::string basef);

    // renders the skybox
    void render();

  protected:

  private:

    // the buffers for the skybox and the texture
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_texture;
    static GLfloat m_verts[108];
};

#endif // SKYBOX_H
