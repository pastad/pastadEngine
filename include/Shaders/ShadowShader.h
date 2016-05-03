#ifndef SHADOWSHADER_H
#define SHADOWSHADER_H

#include <Shader.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

// shader class that renders text

class ShadowShader : public Shader
{
public:
  ShadowShader();
  virtual ~ShadowShader();
  ShadowShader(const ShadowShader& other) = delete;
  ShadowShader& operator=(const ShadowShader& other) = delete;

  // loads the shader
  bool load(const std::string path);

    // setters for matrices
  void setViewMatrix( glm::mat4 mvmatrix );
  void setProjectionMatrix( glm::mat4 projmatrix );
  void setNormalMatrix( glm::mat4 normalmatrix );


  // binds the shader
  void use();

protected:

private:


};

#endif // SHADOWSHADER_H
