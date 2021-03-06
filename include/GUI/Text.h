#ifndef TEXT_H
#define TEXT_H

#include <string>

#include <glm/glm.hpp>

// class for displaying text on screen

class TextShader;

class Text
{

public:
  Text(unsigned int id);
  Text(unsigned int id,std::string txt, glm::vec2 pos, float scale, glm::vec3 color);
  ~Text();

  Text(const Text& other) = delete;
  Text& operator=(const Text& other) = delete;

  // renders the text | shouldn't be called by the user
  void render( TextShader * tex_shader);

  // returns the id of the text
  unsigned int getId();

  // getter and setters
  void set( std::string txt, glm::vec2 pos, float scale, glm::vec3 color);
  void setText(std::string txt);
  std::string getText();
  void setPosition(glm::vec2 p);
  void setColor(glm::vec3 c);
  void setScale(float x);
  void setCentered();
  void setNotCentered();
  void setActive();
  void setInactive();

  // returns the vertical size of the text
  int getSize();

protected:

  // the text positon color and scale
  std::string m_text;
  glm::vec2 m_position;
  glm::vec3 m_color;
  float m_scale; 

  // holds the id
  unsigned int m_id;

  // determines if text is centered
  bool m_centered;

  // determines if text is active
  bool m_active;

  // the vertical length of the text
  int  m_size_vert;

};

#endif // TEXT_H