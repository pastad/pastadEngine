#include "TextShader.h"
#include "Log.h"
#include "Engine.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

TextShader::TextShader():Shader()
{
}

TextShader::~TextShader()
{
}

// load -------------------------------------------------

// modified version of http://learnopengl.com/code_viewer.php?code=in-practice/text_rendering
bool  TextShader::load(const std::string path)
{
  if( ! addShader(path + ".vs", VERTEX) )
    return false;

  if( ! addShader(path + ".fs", FRAGMENT) )
    return false;

  linkAndValidate();

  Engine::getLog()->log("TextShader","loaded");
 
  bind();

  FT_Library ft;
  if (FT_Init_FreeType(&ft))
        Engine::getLog()->log("TextShader", "Couldn't init freetype");
  FT_Face face;
  if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
         Engine::getLog()->log("TextShader", "Couldn't load font");

  FT_Set_Pixel_Sizes(face, 0, 48);
  gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);

  for (GLubyte c = 0; c < 128; c++)
  {
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
          Engine::getLog()->log("TextShader", "Couldn't load glyph");
          continue;
      }

      gl::GenTextures(1, &m_texture);
      gl::BindTexture(gl::TEXTURE_2D, m_texture);
      gl::TexImage2D(
          gl::TEXTURE_2D,
          0,
          gl::RED,
          face->glyph->bitmap.width,
          face->glyph->bitmap.rows,
          0,
          gl::RED,
          gl::UNSIGNED_BYTE,
          face->glyph->bitmap.buffer
      );

      gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
      gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
      gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
      gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);

      Character character = {
          m_texture,
          (GLuint)(face)->glyph->advance.x,
          glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
          glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top)

      };
      m_characters.insert(std::pair<GLchar, Character>(c, character));
  }
  gl::BindTexture(gl::TEXTURE_2D, 0);

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  gl::GenVertexArrays(1, &m_vao);
  gl::GenBuffers(1, &m_vbo);
  gl::BindVertexArray(m_vao);
  gl::BindBuffer(gl::ARRAY_BUFFER, m_vbo);
  gl::BufferData(gl::ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, gl::DYNAMIC_DRAW);
  gl::EnableVertexAttribArray(0);
  gl::VertexAttribPointer(0, 4, gl::FLOAT, gl::FALSE_, 4 * sizeof(GLfloat), 0);
  gl::BindBuffer(gl::ARRAY_BUFFER, 0);
  gl::BindVertexArray(0);

  return true;
}


//  render -------------------------------------------------

// modified from http://learnopengl.com/code_viewer.php?code=in-practice/text_rendering (19.03.2016)
void TextShader::renderText(std::string text, glm::vec2 pos ,float scale, glm::vec3 color, bool centered)
{
  bind();
  setUniform("TextColor",color);
  gl::ActiveTexture(gl::TEXTURE0);
  gl::BindVertexArray(m_vao);

  
  if(centered)
    pos -= (approximateTextOffset(text) / 2.0f *scale);  

  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    Character ch = m_characters[*c];

    GLfloat xpos = pos.x + ch.bearing.x * scale;
    GLfloat ypos = pos.y - (ch.size.y - ch.bearing.y) * scale;

    GLfloat w = ch.size.x * scale;
    GLfloat h = ch.size.y * scale;

    GLfloat vertices[6][4] = {
        { xpos,     ypos + h,   0.0, 0.0 },
        { xpos,     ypos,       0.0, 1.0 },
        { xpos + w, ypos,       1.0, 1.0 },

        { xpos,     ypos + h,   0.0, 0.0 },
        { xpos + w, ypos,       1.0, 1.0 },
        { xpos + w, ypos + h,   1.0, 0.0 }
    };

    gl::BindTexture(gl::TEXTURE_2D, ch.texture_id);

    gl::BindBuffer(gl::ARRAY_BUFFER, m_vbo);
    gl::BufferSubData(gl::ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    gl::BindBuffer(gl::ARRAY_BUFFER, 0);

    gl::DrawArrays(gl::TRIANGLES, 0, 6);

    pos.x += (ch.advance >> 6) * scale;
  }
  gl::BindVertexArray(0);
  gl::BindTexture(gl::TEXTURE_2D, 0);
  checkUniformError("end textshader render");
}


//  getter/setter -------------------------------------------------

void TextShader::setProjection()
{
  bind();
  glm::mat4 projection = glm::ortho(0.0f,(float) Engine::getWindowWidth(), 0.0f, (float)Engine::getWindowHeight());
  setUniform("ProjectionMat", projection);
}


//  approx -------------------------------------------------

glm::vec2 TextShader::approximateTextOffset(std::string text)
{
  float ret = 0.0f;
  float ret_y = 0.0f;
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    Character ch = m_characters[*c] ;
    ret += ch.size.x + ch.bearing.x;
    if(ch.size.y > ret_y)
      ret_y = ch.size.y;
  }
  return glm::vec2(ret,ret_y);
}


//  use -------------------------------------------------

void TextShader::use()
{
  bind();
}



