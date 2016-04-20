#include "Button.h"

#include "Text.h"
#include "Texture.h"

#include "Engine.h"
#include "Log.h"

Button::Button(unsigned int id): Image(id), m_toggl_enable(false),m_toggl(false)
{ 
  m_text = new Text(0);
  m_text->setCentered();
}
Button::~Button()
{  
}

bool Button::intitialize(std::string path, std::string text, glm::vec2 p, glm::vec2 scale,
    glm::vec3 text_color, std::string descriptor)
{
  if(! Image::load(path) )
    return false;
  m_text->setText(text);
  m_descriptor = descriptor;
  m_text->setColor(text_color);
  setPositionAndScale(p,scale);
  return true;
}
bool Button::intitializeWithToggle(std::string path1,std::string path2,
  glm::vec2 p, glm::vec2 scale, std::string descriptor, bool state)
{
  m_toggl_enable = true;
  m_toggl = state;
  m_path_image_on = path1;
  m_path_image_off = path2;
  m_text->setText("");

  if(m_toggl)
    setImage(m_path_image_on);
  else
    setImage(m_path_image_off);
  setPositionAndScale(p,scale);
   m_descriptor = descriptor;
  return true;
}

void Button::render(ImageShader * image_shader, TextShader * text_shader, Quad * quad)
{
  Image::render(image_shader, quad);
  m_text->render(text_shader);
}

void Button::setPositionAndScale(glm::vec2 position, glm::vec2 scale)
{
  Image::setScale(scale);
  Image::setPosition(position);
  m_text->setPosition(position + ( (m_texture->getSize() * scale) /2.0f) );
  m_text->setScale(scale.x);
}
void Button::setTextColor(glm::vec3 color)
{  
  m_text->setColor(color);
}
void Button::setText(std::string text)
{  
  m_text->setText(text);
}
bool Button::wasPressed(float x, float y)
{
  if( ! m_active)
    return false;
  glm::vec2 size  = m_texture->getSize() * m_scale ;
  if (  ( x < (m_position.x +size.x) ) && ( x > m_position.x )  &&
          ( y < (m_position.y +size.y) ) && ( y > m_position.x )      ) 
  {
    //Engine::getLog()->log("Button", "I was pressed");
    if(m_toggl_enable)
    {
      m_toggl = ! m_toggl;
      if(m_toggl)
        setImage(m_path_image_on);
      else
        setImage(m_path_image_off);
    }
    return true;
  }
  else
    return false;
}
std::string Button::getDescriptor()
{
  return m_descriptor;
}
void Button::setDescriptor(std::string desc)
{
  m_descriptor = desc;
}
bool Button::setImage(std::string path)
{
  Image::load(path);
}

bool Button::isToggled()
{
  return m_toggl;
}
bool Button::isTogglEnabled()
{
  return m_toggl_enable;
}