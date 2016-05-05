#ifndef POINTSHADOWSHADER_H
#define POINTSHADOWSHADER_H

#include <Shader.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

// shader class that renders text

class PointShadowShader : public Shader
{
public:
  PointShadowShader();
  virtual ~PointShadowShader();
  PointShadowShader(const PointShadowShader& other) = delete;
  PointShadowShader& operator=(const PointShadowShader& other) = delete;

  // loads the shader
  bool load(const std::string path);

    // setters for matrices
  void setViewMatrix(glm::mat4 view);
  void setLightPosition(glm::vec3 lpos);
  void setProjectionMatrix( glm::mat4 projmatrix );
  void setNormalMatrix( glm::mat4 normalmatrix );


  // binds the shader
  void use();

protected:

private:


};

#endif // POINTSHADOWSHADER_H
