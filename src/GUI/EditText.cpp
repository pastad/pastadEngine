#include "EditText.h"

#include "Engine.h"
#include "IOSubsystem.h"
#include "Log.h"
#include "GUI.h"

#include <iostream>
#include <sstream>
#define TOLERANCE 2

EditText::EditText(unsigned int id, GUI * parent):Text(id)
{  
  m_parent = parent;
}
EditText::EditText(unsigned int id, std::string txt, glm::vec2 pos, float scale, glm::vec3 color, GUI * parent):Text(id,txt,pos,scale,color)
{  
  m_parent = parent;
}
EditText::~EditText()
{  
}

bool EditText::wasPressed(float x, float y)
{
  if( ! m_active)
    return false;
  
  m_size.x = Text::getSize();
  if (  ( x < (m_position.x +Text::getSize()+TOLERANCE ) ) && ( x > m_position.x - TOLERANCE)  &&
          ( y < (m_position.y +m_size.y) ) && ( y > m_position.y )      ) 
  {
    Engine::getLog()->log("EditText" ,"was pressed"); 
    Engine::setGUIMovementLock();
    m_input_mode = true; 

   // std::cout <<m_position.x <<std::endl;    
    //std::cout <<m_size.x <<std::endl;
    //std::cout <<x <<std::endl;
    //std::cout <<m_size_vert <<std::endl;

    return true;
  }
  else
  { 
    return false;
  }
}

void EditText::refreshSize()
{
  Text::m_size_vert =-1;
}
void EditText::setText(std::string txt)
{
  Text::setText(txt);
  Text::m_size_vert =-1;
}

bool EditText::handleInput(unsigned int key)
{
  if( ( key == GLFW_KEY_ENTER) || (key == GLFW_KEY_KP_ENTER ) )
  {
    m_input_mode = false;
    Engine::unsetGUIMovementLock();
    refreshSize();
    return true;
  }
  else
  {
    if( key == GLFW_KEY_BACKSPACE)
    {
      if(m_text.size() >0)
        m_text.pop_back();
    }
    else
    {
      char k = (char)key; 
      if(  ( key >= 65 ) && ( key <91 )  ) // differentiate captitol and lower case
      {
        if( ! IOSubsystem::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        {
          k = (char) (key+32);
        }
        std::cout << k <<std::endl;
        std::stringstream ss;
        ss << m_text << k;
        m_text = ss.str();
      }
      else
      {
        if( ( key >= 320 ) && ( key <329 ) )  // numpad numbers
        {
           k = (char) (key-272);
        }
        if(key == GLFW_KEY_KP_DECIMAL )
          k = '.';
        if(key == GLFW_KEY_KP_ADD )
          k = '+';
        if(key == GLFW_KEY_KP_SUBTRACT )
          k = '-';
        if(key == GLFW_KEY_KP_DIVIDE )
          k = '/';
        if(key == GLFW_KEY_KP_MULTIPLY )
          k = '*';
        //std::cout << k <<std::endl;
        std::stringstream ss;
        ss << m_text << k;
        m_text = ss.str();
      }
    }
  }
  return false;
}
bool EditText::isInInputMode()
{
  return m_input_mode;
}
void EditText::setDescriptor(std::string descriptor)
{
  m_descriptor = descriptor;
}
std::string EditText::getDescriptor()
{
  return m_descriptor;
}

GUI * EditText::getParent()
{
  return m_parent;
}