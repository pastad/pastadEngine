#include "GUI.h"

#include "Text.h"
#include "EditText.h"
#include "TextShader.h"
#include "Image.h"
#include "ImageShader.h"
#include "Button.h"

#include "Engine.h"
#include "Log.h"

#include <iostream>

GUI::GUI(unsigned int id):m_id(id),m_text_ids(0),m_button_ids(0),m_active(true)
{  
  external_buttonPressedCallback =  nullptr;
  external_editTextCallback = nullptr;
}
GUI::~GUI()
{  
  for( std::map<int,Text*>::iterator it = m_texts.begin(); it != m_texts.end(); it++)
  { 
    delete (it->second);
  }
  m_texts.clear();
  for( std::map<int,EditText*>::iterator it = m_edit_texts.begin(); it != m_edit_texts.end(); it++)
  { 
    delete (it->second);
  }
  m_edit_texts.clear();
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
    for( std::map<int,EditText*>::iterator it = m_edit_texts.begin(); it != m_edit_texts.end();it++)
      it->second->render(text_shader);
    for(int x =0; x< m_texts.size();x++)
      m_texts.at(x)->render(text_shader);
    for( std::vector<GUI*>::iterator it = m_children.begin(); it != m_children.end();it++)
      (*it)->render(text_shader,image_shader,quad);
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
EditText * GUI::getEditText()
{
  EditText * text = new EditText(m_edit_text_ids, this);
  m_edit_texts.insert(std::pair<int, EditText*>(m_edit_text_ids,text));
  m_edit_text_ids++;
  return text;
}

void GUI::removeEditText(EditText * text)
{  
  std::map<int,EditText*>::iterator it = m_edit_texts.find(text->getId());
  if(it != m_edit_texts.end())
  {
    delete it->second;
    m_edit_texts.erase(it); 
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
  Button * button = new Button(m_button_ids, this);
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
  //Engine::getLog()->log("GUI", "btn check");
  if(!m_active)
    return false;

  bool ret = false;
  for( std::map<int,Button*>::iterator it = m_buttons.begin(); it != m_buttons.end();it++)
  {
    if( it->second->wasPressed(x,y)  &&  it->second->isActive() )
    {
      ret = true;
      if( external_buttonPressedCallback != nullptr )
        external_buttonPressedCallback(it->second);
    }
  }
  for( std::map<int,EditText*>::iterator it = m_edit_texts.begin(); it != m_edit_texts.end();it++)
  {
    if( it->second->wasPressed(x,y)  )
    {
      ret = true;    
    }
  }

  for( std::vector<GUI *>::iterator it = m_children.begin(); it != m_children.end();it++)
  {
    if( (*it)->checkButtonPressed(x,y) &&  (*it)->isActive()  )
      ret = true;
  }

  return ret;
}

void GUI::keyWasPressed(unsigned int key)
{
  for( std::map<int,EditText*>::iterator it = m_edit_texts.begin(); it != m_edit_texts.end();it++)
  {
    if(it->second->isInInputMode())
    {
      if(it->second->handleInput(key))
      {
        if( external_editTextCallback != nullptr)
          external_editTextCallback(it->second);
      }
    }
  }
  for( std::vector<GUI *>::iterator it = m_children.begin(); it != m_children.end();it++)
  {
    (*it)->keyWasPressed(key);
  }
}

void GUI::registerButtonPressedCallback( void   (*callback)(Button * btn)  )
{
  external_buttonPressedCallback = callback;
}

void GUI::registerEditTextCallback( void  (*callback)(EditText * btn)   )
{
  external_editTextCallback = callback;
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

void GUI::addChild(GUI * gui)
{
  if( gui != nullptr)
    m_children.insert(m_children.end(),gui);
}
bool GUI::isInside(glm::vec2 pos)
{
  return (  ( pos.x < (m_offset.x +m_size.x) ) && ( pos.x > m_offset.x )  &&
          ( pos.y < (m_offset.y +m_size.y) ) && ( pos.y > m_offset.y )      ) ;
}
void GUI::setSizeAndOffset(glm::vec2 size, glm::vec2 offset)
{
  m_size = size;
  m_offset = offset;
}