#include "Text.h"

#include "RenderSubsystem.h"
#include "TextShader.h"


Text::Text():m_text("DEFAULT TEXT"),m_position(0,0,0),m_scale(1.0f),m_color(1,1,1)
{  
}
Text::Text(std::string txt, glm::vec3 pos, float scale, glm::vec3 color): m_text(txt),m_position(pos),m_scale(scale),m_color(color)
{  
}
Text::~Text()
{  
}

void Text::render(TextShader * text_shader)
{
  if(text_shader != nullptr)
  {
    text_shader->setProjection();
    text_shader->renderText(m_text,m_position,m_scale,m_color);
  }
}