#ifndef RENDER_BASE_SHADER_H
#define RENDER_BASE_SHADER_H

#define MAX_NUM_MATERIALS 100

// the base shader for rendering shaders

#include <string>
#include <vector>

#include "Shader.h"
#include "Engine.h"

class Light;

class RenderBaseShader :  public Shader
{
public:

  RenderBaseShader();
  ~RenderBaseShader(); 

  // loads the shader
  bool load(const std::string path);

  // ready query
  using Shader::isReady;
  using Shader::addShader;
  using Shader::linkAndValidate;  
  using Shader::bind;
  using Shader::setUniform;
  using Shader::setRenderPassSubroutine;
  using Shader::checkError;
  using Shader::checkUniformError;


  // uniform setters -----------------------------------------------

  // setters for matrices
  void setViewMatrix( glm::mat4 mvmatrix );
  void setProjectionMatrix( glm::mat4 projmatrix );
  void setNormalMatrix( glm::mat4 normalmatrix );

  // animation setters -------------------------------------------

  // enables animation
  void setAnimation();

  // diables animation
  void unsetAnimation();

  void setBones(std::vector<glm::mat4> * transforms);


  // instanced setters -------------------------------------------

  // enables instancing
  void setInstanced();

  // enables single render
  void setNotInstanced(glm::mat4 model_transform); 

  // general functions --------------------------------------------  

  // sets the object id for picking purposes
  void setIndentifcation(int idx);

  // binds the shader
  void use();
  
private:


};

#endif