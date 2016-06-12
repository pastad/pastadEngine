#include  "PostProcessingShader.h"

#include "Engine.h"
#include "Log.h"
#include "Helper.h"

#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"


#include <iostream>
#include <sstream>

PostProcessingShader::PostProcessingShader() : Shader()
{
}
PostProcessingShader::~PostProcessingShader()
{
}

bool PostProcessingShader::load(const std::string path)
{  
  if( ! addShader(path + ".vs", VERTEX) )
    return false;

  if( ! addShader(path + ".fs", FRAGMENT) )
    return false;

  linkAndValidate();

  Engine::getLog()->log("PostProcessingShader","loaded");

  return true;
}

void PostProcessingShader::setRenderPass(unsigned int pass)
{
  bind();
  if(pass == 1)
  {
    setRenderPassSubroutine("passStandard");
  }
  if(pass == 2)
  {
    setRenderPassSubroutine("passBright");
  }
  if(pass == 3)
  {
    setRenderPassSubroutine("passBlur");
  }
  if(pass == 4)
  {
    setRenderPassSubroutine("passBlur2");
  }

  checkUniformError("set Subroutine");
  //printUniforms();
}

void PostProcessingShader::use()
{
  Shader::bind();
}

void PostProcessingShader::setIdentityMatrices()
{
  glm::mat4 view = glm::mat4(1.0);
  glm::mat4 projection = glm::mat4(1.0);
  setUniform("ViewMat",view);
  setUniform("ProjectionMat",projection);
  checkUniformError("pp view projection set");
}


void PostProcessingShader::setTextureScaling(glm::vec2 scale)
{
  setUniform("TextureScale",scale);    
  checkUniformError("texture scale set");
}

void PostProcessingShader::setFXAA(bool enable)
{
  if(enable)
    setUniform("EnableFXAA",1);
  else
    setUniform("EnableFXAA",0);
}
void PostProcessingShader::setHDR(bool enable)
{
  if(enable)
    setUniform("EnableHDR",1);
  else
    setUniform("EnableHDR",0);
}
void PostProcessingShader::setBloom(bool enable)
{
  if(enable)
    setUniform("EnableBloom",1);
  else
    setUniform("EnableBloom",0);
}

void PostProcessingShader::setBloomThreshold(float val)
{
  setUniform("BloomThreshold", val);
}

void PostProcessingShader::setAverageLuminance(float value)
{
  setUniform("AverageLuminance",value);    
  checkUniformError("AverageLuminance set");
}
void PostProcessingShader::setExposure(float value)
{
  setUniform("Exposure",value);    
  checkUniformError("Exposure set");
}
void PostProcessingShader::setGaussSize(unsigned int size)
{
  bind();
  float weights[size];

  float sum =0.0f;
  for(int count = 1; count< size+1;count++)
  {
    weights[count-1] = Helper::gauss(float(count), 25.0f);
    sum += 2 * weights[count-1];
  }
  for(int count = 1; count< size+1;count++)
  {
    std::stringstream ss, ssi;
    ss << "GaussKernel["<<count -1<<"]";
    setUniform(ss.str(), weights[count-1]/sum);    
    checkUniformError("GaussKernel set");
  }
}