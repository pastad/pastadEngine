#include "Text.h"

#include "RenderSubsystem.h"
#include "TextShader.h"


Text::Text(unsigned int id):m_id(id),m_text("DEFAULT TEXT"),m_position(0,0),m_scale(1.0f),m_color(1,1,1),m_centered(false),m_active(true)
{  
}
Text::Text(unsigned int id, std::string txt, glm::vec2 pos, float scale, glm::vec3 color):m_id(id), m_text(txt),m_position(pos),m_scale(scale),m_color(color), m_centered(false),m_active(true)
{  
}
Text::~Text()
{  
}

void Text::render(TextShader * text_shader)
{
  if((text_shader != nullptr) && m_active )
  {
    text_shader->setProjection();
    text_shader->renderText(m_text,m_position,m_scale,m_color, m_centered);
  }
}
unsigned int Text::getId()
{
  return m_id;
}

void Text::set( std::string txt, glm::vec2 pos, float scale, glm::vec3 color)
{
  m_text = txt;
  m_position = pos;
  m_scale = scale;
  m_color = color;
}
void Text::setText(std::string txt)
{
  m_text = txt;
}
void Text::setPosition(glm::vec2 p)
{
  m_position = p;
}
void Text::setScale(float scale)
{
  m_scale = scale;
}
void Text::setColor(glm::vec3 color )
{
  m_color = color;
}
void Text::setCentered()
{
  m_centered = true;
}
void Text::setNotCentered()
{
  m_centered = false;
}
void Text::setActive()
{
  m_active = true;
}
void Text::setInactive()
{
  m_active = false;
}