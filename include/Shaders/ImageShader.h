#ifndef IMAGESHADER_H
#define IMAGESHADER_H

#include <Shader.h>
#include <glm/glm.hpp>
#include <map>

// shader class that renders text

class ImageShader : public Shader
{
public:
  ImageShader();
  virtual ~ImageShader();
  ImageShader(const ImageShader& other) = delete;
  ImageShader& operator=(const ImageShader& other) = delete;

  // loads the shader
  bool load(const std::string path);
  
  // sets the position
  void setPosition( glm::vec2 pos );
  void setScreenSize( glm::vec2 pos );
  void setModelSize( glm::vec2 sz );

  // binds the shader
  void use();

protected:

private:


};

#endif // IMAGESHADER_H
