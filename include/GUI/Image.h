#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

// class represents a image (texture rendered on quad)

class Texture;
class ImageShader;
class Quad;

class Image
{
public:
  Image(unsigned int id);
  ~Image();

  Image(const Image& other) = delete;
  Image& operator=(const Image& other) = delete;

  // returns the id of the image
  unsigned int getId();

  // loads the image
  bool load(std::string path);

  // renders the image using the shader
  void render(ImageShader * image_shader, Quad * quad);

  // sets the position of the image
  void setPosition(glm::vec2 position);

  // sets the scale
  void setScale(glm::vec2 scale);

  // active setters and getter
  void setActive();
  void setInactive();
  bool isActive();

  // returns the size of the image
  glm::vec2 getSize();

protected:

  // the id of the image
  unsigned int m_id;

  // holds the texture of the image
  Texture * m_texture;

  // holds the position and scale
  glm::vec2 m_position;
  glm::vec2 m_scale;

  // activation state 
  bool m_active;


  
};

#endif // IMAGE_H