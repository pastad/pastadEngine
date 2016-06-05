#ifndef SKYBOX_H
#define SKYBOX_H

#include "Engine.h"
#include <string>
#include <glm/glm.hpp>

#include "tinyxml2.h"

// class to represent the skybox

class Skybox
{
  public:
    Skybox();
    ~Skybox();
    Skybox(const Skybox& other) = delete;
    Skybox& operator=(const Skybox& other) = delete;

    // loads the basic skybox
    bool load(const std::string basef);

    // renders the skybox
    void render();

    //saves the skybox
    void save(tinyxml2::XMLNode * parent, tinyxml2::XMLDocument* document);

    // loads the skybox from xml
    bool load( tinyxml2::XMLElement *  element);

  protected:

  private:

    // the buffers for the skybox and the texture
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_texture;
    static GLfloat m_verts[108];

    // the base file of the skybox
    std::string m_base_file;
};

#endif // SKYBOX_H
