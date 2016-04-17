#ifndef TEXT_H
#define TEXT_H


#include <string>

#include <glm/glm.hpp>

class TextShader;

class Text
{

public:
  Text();
  Text(std::string txt, glm::vec3 pos, float scale, glm::vec3 color);
  ~Text();

  Text(const Text& other) = delete;
  Text& operator=(const Text& other) = delete;

  // renders the text | shouldn't be called by the user
  void render( TextShader * tex_shader);

protected:

  // the text positon color and scale
  std::string m_text;
  glm::vec3 m_position;
  glm::vec3 m_color;
  float m_scale; 

};

#endif // TEXT_H