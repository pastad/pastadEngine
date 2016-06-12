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

  // sets the average luminance for HDR
  void setAverageLuminance(float value);

  // sets the gauss size for bluring
  void setGaussSize(unsigned int size);

  // sets the exposure for HDR
  void setExposure(float value);

  //sets the basic matrices
  void setIdentityMatrices();

  // pass true if fxaa should be enabled
  void setFXAA(bool enable);

  // pass true if HDR should be enabled
  void setHDR(bool enable);

   // pass true if Bloom should be enabled
  void setBloom(bool enable);

  // sets the minimal bloom lum value
  void setBloomThreshold(float val);

  // binds the shader  
  void use();
  
};

#endif //POST_PROCESSING_SHADER_H