#ifndef SKYBOXSHADER_H
#define SKYBOX_H

#include <Shader.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

// shader to render the skybox

class SkyboxShader : public Shader
{
  public:
    SkyboxShader();
    ~SkyboxShader();
    SkyboxShader(const SkyboxShader& other) = delete;
    SkyboxShader& operator=(const SkyboxShader& other) = delete;

    // loads the shader
    bool load(const std::string path);

    // sets the view and projection matrix
    void setView(glm::mat4 view); 
    void setProjection(glm::mat4 projection);

    // binds the shader
    void use();

  protected:

  private:

  
};

#endif // SKYBOX_H
