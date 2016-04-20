#ifndef TEXTSHADER_H
#define TEXTSHADER_H

#include <Shader.h>
#include <glm/glm.hpp>
#include <map>

// shader class that renders text

class TextShader : public Shader
{
public:
  TextShader();
  virtual ~TextShader();
  TextShader(const TextShader& other) = delete;
  TextShader& operator=(const TextShader& other) = delete;

  // loads the shader
  bool load(const std::string path);

  // renders the text
  void renderText(std::string txt, glm::vec2 pos ,float scale, glm::vec3 color, bool centered);
  
  // sets the right projection matrix
  void setProjection();

  // returns the text size
  glm::vec2 approximateTextOffset(std::string text);

protected:

private:

  // the text texture
  GLuint m_texture;

  // represents a character 
  struct Character
  {
    GLuint texture_id;
    GLuint advance;
    glm::ivec2 size;
    glm::ivec2 bearing;
  };

  // a map of characters
  std::map<GLchar, Character> m_characters;

  // the buffer handles
  GLuint m_vao;
  GLuint m_vbo;
};

#endif // TEXTSHADER_H
