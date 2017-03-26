#include  "PostProcessingShader.h"

#include "Engine.h"
#include "Log.h"
#include "Helper.h"

#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"


#include <iostream>
#include <sstream>
#include <random>

PostProcessingShader::PostProcessingShader() : Shader()
{
}

PostProcessingShader::~PostProcessingShader()
{
}


//  load -------------------------------------------------

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


//  getter/setter -------------------------------------------------

void PostProcessingShader::setRenderPass(unsigned int pass)
{
  bind();
  if(pass == PASS_STANDARD)
  {
    setRenderPassSubroutine("passStandard");
  }
  if(pass == PASS_BRIGHT)
  {
    setRenderPassSubroutine("passBright");
  }
  if(pass == PASS_BLUR_1)
  {
    setRenderPassSubroutine("passBlur");
  }
  if(pass == PASS_BLUR_2)
  {
    setRenderPassSubroutine("passBlur2");
  }
  if(pass == PASS_SSAO)
  {
    setRenderPassSubroutine("passSSAO");
  }

  checkUniformError("set Subroutine");
  //printUniforms();
}

void PostProcessingShader::setIdentityMatrices()
{
  glm::mat4 view = glm::mat4(1.0);
  glm::mat4 projection = glm::mat4(1.0);
  setUniform("ViewMat",view);
  setUniform("ProjectionMat",projection);
  checkUniformError("pp view projection set");
}

void PostProcessingShader::setProjectionMatrix(glm::mat4 proj)
{
   setUniform("CameraProjection",proj);
}

void PostProcessingShader::setViewMatrix(glm::mat4 view)
{
  setUniform("CameraView",view);
}

void PostProcessingShader::setCameraPosition(glm::vec3 coord)
{
  setUniform("CameraPosition", coord);
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
  float * weights = new float[size];

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
    setUniform(ss.str() , (float )(weights[count-1]/sum) );    
    checkUniformError("GaussKernel set");
  }
  delete weights;
}

void PostProcessingShader::setSSAOSamples()
{
  std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
  std::default_random_engine gen;
  for (GLuint i = 0; i < 64; ++i)
  {
    std::stringstream ss;
    float offset = 0.3;
    float x = randomFloats(gen) * ( 2.0 - (2*offset) )  - (1.0 -offset) ;
    float y = randomFloats(gen) * (2.0 - (2 * offset)) - (1.0 - offset);
    float z = randomFloats(gen) -offset;
   
    glm::vec3 one_samle( x,y,z );

    one_samle = glm::normalize(one_samle);
    one_samle *= randomFloats(gen);
    GLfloat scale = GLfloat(i) / 64.0;

    scale = Helper::lerp(0.1f, 1.0f, scale * scale);
    one_samle *= scale;
    ss<< "SSAOSamples["<<i<<"]";
    std::cout << one_samle.x <<","<<one_samle.y<<","<<one_samle.z<<std::endl;
    setUniform(ss.str(), one_samle);
  }

}

void PostProcessingShader::setEmmissiveIndices(std::vector<int> indices)
{
  for(int x=0; x< 20;x++)
  {
    std::stringstream ss;
    ss<< "Emissive["<<x<<"]";
    setUniform(ss.str(),0);
  }
  for(int x=0; x< indices.size();x++)
  {
    std::stringstream ss;
    ss<< "Emissive["<<indices[x]<<"]";
    setUniform(ss.str(),1);
    //std::cout << indices[x]<<std::endl;
  }
}


//  use -------------------------------------------------

void PostProcessingShader::use()
{
  Shader::bind();
}
