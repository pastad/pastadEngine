#include "GUI.h"

#include "Text.h"
#include "TextShader.h"

GUI::GUI(unsigned int id):m_id(id)
{  
}
GUI::~GUI()
{  
}

void GUI::render(TextShader * text_shader)
{
  if(m_active)
  {
    for(int x =0; x< m_texts.size();x++)
      m_texts.at(x)->render(text_shader);
  }
}
void GUI::setActive()
{
  m_active = true;
}
void GUI::setInactive()
{
  m_active = false;
}