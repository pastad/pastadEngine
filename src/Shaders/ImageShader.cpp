#include "ImageShader.h"
#include "Log.h"
#include "Engine.h"


#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

ImageShader::ImageShader():Shader()
{
}

ImageShader::~ImageShader()
{
}


//  load -------------------------------------------------

bool ImageShader::load(const std::string path)
{
  if( ! addShader(path + ".vs", VERTEX) )
    return false;

  if( ! addShader(path + ".fs", FRAGMENT) )
    return false;

  linkAndValidate();

  Engine::getLog()->log("ImageShader","loaded"); 

  return true;
}


//  getters/setters -------------------------------------------------

void ImageShader::setPosition( glm::vec2 positon )
{
  bind();
  setUniform("ModelPosition",positon);
  checkUniformError("set ModelPosition"); 
}

void ImageShader::setScreenSize( glm::vec2 sz )
{
  bind();
  setUniform("ScreenSize",sz);
  checkUniformError("set ScreenSize"); 
}

void ImageShader::setModelSize( glm::vec2 sz )
{
  bind();
  setUniform("ModelSize",sz);
  checkUniformError("set ModelSize"); 
}

void ImageShader::setMixColor(glm::vec3 color, float value)
{
  bind();
  setUniform("MixColor",color);
  setUniform("MixValue", value);
}

//  use -------------------------------------------------

void ImageShader::use()
{
  bind();
}



