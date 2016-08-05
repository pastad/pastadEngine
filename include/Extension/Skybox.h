#ifndef SKYBOX_H
#define SKYBOX_H

#include "Engine.h"
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "tinyxml2.h"

class SkyboxShader;

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
    void render(SkyboxShader * shader);

    //saves the skybox
    void save(tinyxml2::XMLNode * parent, tinyxml2::XMLDocument* document);

    // loads the skybox from xml
    bool load( tinyxml2::XMLElement *  element);

    // sets the light strength for the skybox
    void setLightStrength(float strength);

  protected:

  private:

    // the buffers for the skybox and the texture
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_texture;
    static GLfloat m_verts[108];

    // the base file of the skybox
    std::string m_base_file;

    // the light strength
    float m_light_strength;
};

#endif // SKYBOX_H
