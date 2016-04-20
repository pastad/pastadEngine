#include "GUI.h"

#include "Text.h"
#include "TextShader.h"
#include "Image.h"
#include "ImageShader.h"
#include "Button.h"

GUI::GUI(unsigned int id):m_id(id),m_text_ids(0),m_button_ids(0),m_active(true)
{  
  external_buttonPressedCallback = nullptr;
}
GUI::~GUI()
{  
  for( std::map<int,Text*>::iterator it = m_texts.begin(); it != m_texts.end(); it++)
  { 
    delete (it->second);
  }
  m_texts.clear();
  for( std::map<int,Image*>::iterator it = m_images.begin(); it != m_images.end();it++)
  { 
    delete (it->second);
  }
  m_images.clear();
  for( std::map<int,Button*>::iterator it = m_buttons.begin(); it != m_buttons.end();it++)
  { 
    delete (it->second);
  }
  m_buttons.clear();
}

void GUI::render(TextShader * text_shader, ImageShader* image_shader, Quad * quad)
{
  if(m_active)
  {    
    for( std::map<int,Image*>::iterator it = m_images.begin(); it != m_images.end();it++)
      it->second->render(image_shader, quad);
    for( std::map<int,Button*>::iterator it = m_buttons.begin(); it != m_buttons.end();it++)
      it->second->render(image_shader,text_shader, quad);
    for(int x =0; x< m_texts.size();x++)
      m_texts.at(x)->render(text_shader);
  }
}

Text * GUI::getText()
{
  Text * text = new Text(m_text_ids);
  m_texts.insert(std::pair<int, Text*>(m_text_ids,text));
  m_text_ids++;
  return text;
}
void GUI::removeText(Text * text)
{  
  std::map<int,Text*>::iterator it = m_texts.find(text->getId());
  if(it != m_texts.end())
  {
    delete it->second;
    m_texts.erase(it); 
  }
}
Image * GUI::getImage()
{
  Image * image = new Image(m_image_ids);
  m_images.insert(std::pair<int, Image*>(m_image_ids,image));
  m_image_ids++;
  return image;
}
void GUI::removeImage(Image * image)
{  
  std::map<int,Image*>::iterator it = m_images.find(image->getId());
  if(it != m_images.end())
  {
    delete it->second;
    m_images.erase(it); 
  }
}
Button * GUI::getButton()
{
  Button * button = new Button(m_button_ids);
  m_buttons.insert(std::pair<int, Button*>(m_button_ids,button));
  m_button_ids++;
  return button;
}
void GUI::removeButton(Button * button)
{  
  std::map<int,Button*>::iterator it = m_buttons.find(button->getId());
  if(it != m_buttons.end())
  {
    delete it->second;
    m_buttons.erase(it); 
  }
}

bool GUI::checkButtonPressed(float x, float y)
{
  bool ret = false;
  for( std::map<int,Button*>::iterator it = m_buttons.begin(); it != m_buttons.end();it++)
  {
    if( it->second->wasPressed(x,y) )
    {
      ret = true;
      if( external_buttonPressedCallback != nullptr )
        external_buttonPressedCallback(it->second);
    }
  }

  return ret;
}

void GUI::registerButtonPressedCallback( void   (*callback)(Button * btn)  )
{
  external_buttonPressedCallback = callback;
}

void GUI::setActive()
{
  m_active = true;
}
void GUI::setInactive()
{
  m_active = false;
}
bool GUI::isActive()
{
  return m_active;
}

unsigned int GUI::getId()
{
  return m_id;
}