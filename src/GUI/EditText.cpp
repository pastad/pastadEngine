#include "EditText.h"

#include "Engine.h"
#include "IOSubsystem.h"
#include "Log.h"

#include <iostream>
#include <sstream>

EditText::EditText(unsigned int id):Text(id)
{  
}
EditText::EditText(unsigned int id, std::string txt, glm::vec2 pos, float scale, glm::vec3 color):Text(id,txt,pos,scale,color)
{  
}
EditText::~EditText()
{  
}

bool EditText::wasPressed(float x, float y)
{
  if( ! m_active)
    return false;
  if (  ( x < (m_position.x +m_size.x) ) && ( x > m_position.x )  &&
          ( y < (m_position.y +m_size.y) ) && ( y > m_position.y )      ) 
  {
    Engine::getLog()->log("EditText" ,"was pressed"); 
    Engine::setGUIMovementLock();
    m_input_mode = true; 
    return true;
  }
  else
  { 
    return false;
  }
}
bool EditText::handleInput(unsigned int key)
{
  if( ( key == GLFW_KEY_ENTER) || (key == GLFW_KEY_KP_ENTER ) )
  {
    m_input_mode = false;
    Engine::unsetGUIMovementLock();
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