#ifndef POST_PROCESSING_SHADER_H
#define POST_PROCESSING_SHADER_H

// shader for post processing passes

#include <string>
#include <vector>

#include "Shader.h"



class PostProcessingShader :  public Shader
{
public:

  PostProcessingShader();
  ~PostProcessingShader(); 

  // loads the shader
  bool load(const std::string path);

  // sets the render pass
  void setRenderPass(unsigned int pass);

  // sets the scale to scale the texture in the shader
  void setTextureScaling(glm::vec2 scale);

  //sets the basic matrices
  void setIdentityMatrices();

  // pass true if fxaa should be enabled
  void setFXAA(bool enable);

  // binds the shader  
  void use();
  
};

#endif //POST_PROCESSING_SHADER_H